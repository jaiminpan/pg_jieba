/*-------------------------------------------------------------------------
 *
 * pg_jieba.c
 *    a text search parser for Chinese
 *
 * Author: Jaimin Pan <jaimin.pan@gmail.com>
 *
 * IDENTIFICATION
 *    pg_jieba.cpp
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "lib/stringinfo.h"
#include "utils/guc.h"
#include "utils/elog.h"
#include "utils/varlena.h"
#include "miscadmin.h"
#include "tsearch/ts_public.h"

#include "jieba.h"

PG_MODULE_MAGIC;

/* Start From 1 and LASTNUM is the last number */
int LASTNUM = sizeof(lex_descr) / sizeof(lex_descr[0]) - 1;

/*
 * types
 */
typedef struct
{
	char	   *buffer;			/* text to parse */
	int			len;			/* length of the text in buffer */
	JiebaCtx   *ctx;
	ParStat    *stat;
} ParserState;


void _PG_init(void);
void _PG_fini(void);

/*
 * prototypes
 */
PG_FUNCTION_INFO_V1(jieba_start);
Datum jieba_start(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_query_start);
Datum jieba_query_start(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_mp_start);
Datum jieba_mp_start(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_hmm_start);
Datum jieba_hmm_start(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_gettoken);
Datum jieba_gettoken(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_end);
Datum jieba_end(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_lextype);
Datum jieba_lextype(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_reload_dict);
Datum jieba_reload_dict(PG_FUNCTION_ARGS);


#define DICT_EXT "dict"
#define MODEL_EXT "model"

static void DefineCustomConfigVariables();
static void recompute_dicts_path(void);
static char* extract_dict_list(const char *dictsString);
static char* jieba_get_tsearch_config_filename(const char *basename, const char *extension);

static JiebaCtx* jieba = NULL;

/* GUC variables */
static char *pg_jieba_hmm_model = "jieba_hmm";
static char *pg_jieba_dict = "jieba_base";
static char *pg_jieba_user_dict = "jieba_user";

/* These variables are the values last set */
static char *userDicts = NULL;

/* The above values are valid only if userDictsValid */
static bool userDictsValid = true;

static bool check_user_dict(char **newval, void **extra, GucSource source);
static void assign_user_dict(const char *newval, void *extra);


/*
 * Module load callback
 */
void
_PG_init(void)
{
	if (!process_shared_preload_libraries_in_progress)
	{
		ereport(LOG,
				(errmsg("pg_jieba Extension is not loaded by shared_preload_libraries, "
						"Variables can't be configured")));
	}
	else
	{
		DefineCustomConfigVariables();
	}

	userDictsValid = false;
	recompute_dicts_path();
}

/*
 * Module unload callback
 */
void
_PG_fini(void)
{
	if (jieba) {
		Jieba_Free(jieba);
		jieba = NULL;
	}
}

/*
 * functions
 */
Datum
jieba_start(PG_FUNCTION_ARGS)
{
	ParserState* const pst = (ParserState *) palloc0(sizeof(ParserState));
	pst->buffer = (char *) PG_GETARG_POINTER(0);
	pst->len = PG_GETARG_INT32(1);

	pst->ctx = jieba;

	pst->stat = Jieba_Cut(pst->ctx, pst->buffer, pst->len, MODE_MIX);

	PG_RETURN_POINTER(pst);
}

Datum
jieba_query_start(PG_FUNCTION_ARGS)
{
	ParserState* const pst = (ParserState *) palloc0(sizeof(ParserState));
	pst->buffer = (char *) PG_GETARG_POINTER(0);
	pst->len = PG_GETARG_INT32(1);

	pst->ctx = jieba;

	pst->stat = Jieba_Cut(pst->ctx, pst->buffer, pst->len, MODE_QRY);

	PG_RETURN_POINTER(pst);
}

Datum
jieba_mp_start(PG_FUNCTION_ARGS)
{
	ParserState* const pst = (ParserState *) palloc0(sizeof(ParserState));
	pst->buffer = (char *) PG_GETARG_POINTER(0);
	pst->len = PG_GETARG_INT32(1);

	pst->ctx = jieba;

	pst->stat = Jieba_Cut(pst->ctx, pst->buffer, pst->len, MODE_MP);

	PG_RETURN_POINTER(pst);
}

Datum
jieba_hmm_start(PG_FUNCTION_ARGS)
{
	ParserState* const pst = (ParserState *) palloc0(sizeof(ParserState));
	pst->buffer = (char *) PG_GETARG_POINTER(0);
	pst->len = PG_GETARG_INT32(1);

	pst->ctx = jieba;

	pst->stat = Jieba_Cut(pst->ctx, pst->buffer, pst->len, MODE_HMM);

	PG_RETURN_POINTER(pst);
}

Datum
jieba_gettoken(PG_FUNCTION_ARGS)
{
	ParserState* const pst = (ParserState *) PG_GETARG_POINTER(0);
	char	  **t = (char **) PG_GETARG_POINTER(1);
	int		   *tlen = (int *) PG_GETARG_POINTER(2);
	int			type = -1;

	JiebaResult* curr = Jieba_GetNext(pst->ctx, pst->stat);

	/* already done the work, or no sentence */
	if (curr == NULL)
	{
		*tlen = 0;
		type = 0;

		PG_RETURN_INT32(type);
	}

	type = curr->attr;
	*tlen = curr->len;
	*t = curr->str;

	PG_RETURN_INT32(type);
}

Datum
jieba_end(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);

	if (pst->stat) {
		ParStat_Free(pst->stat);
		pst->stat = NULL;
	}

	pfree(pst);
	PG_RETURN_VOID();
}

Datum
jieba_lextype(PG_FUNCTION_ARGS)
{
	int			i;
	int			size = LASTNUM;
	LexDescr   *descr = (LexDescr *) palloc(sizeof(LexDescr) * (size + 1));

	for (i = 1; i <= size; i++)
	{
		descr[i - 1].lexid = i;
		descr[i - 1].alias = pstrdup(lex_descr[i].token);
		descr[i - 1].descr = pstrdup(lex_descr[i].descr);
	}

	descr[size].lexid = 0;

	PG_RETURN_POINTER(descr);
}

Datum
jieba_reload_dict(PG_FUNCTION_ARGS)
{
    userDictsValid = false;
    recompute_dicts_path();
	PG_RETURN_VOID();
}

static void
DefineCustomConfigVariables()
{
	DefineCustomStringVariable("pg_jieba.hmm_model",
							"hmm model file.",
							NULL,
							&pg_jieba_hmm_model,
							"jieba_hmm",
							PGC_POSTMASTER, 0,
							NULL,
							NULL,
							NULL);

	DefineCustomStringVariable("pg_jieba.base_dict",
							"base dictionary.",
							NULL,
							&pg_jieba_dict,
							"jieba_base",
							PGC_POSTMASTER, 0,
							NULL,
							NULL,
							NULL);

	DefineCustomStringVariable("pg_jieba.user_dict",
							"CSV list of specific user dictionary.",
							NULL,
							&pg_jieba_user_dict,
							"jieba_user",
							PGC_POSTMASTER, 0,
							check_user_dict,
							assign_user_dict,
							NULL);
}

static void
recompute_dicts_path(void)
{
	MemoryContext oldcxt;
	char	   *user_dicts;
	char	   *new_dicts;

	JiebaCtx   *new_jieba = NULL;
	char* dict_path;
	char* hmm_model_path;

	/* Do nothing if path is already valid. */
	if (userDictsValid)
		return;

	dict_path = jieba_get_tsearch_config_filename(pg_jieba_dict, DICT_EXT);
	hmm_model_path = jieba_get_tsearch_config_filename(pg_jieba_hmm_model, MODEL_EXT);
	user_dicts = extract_dict_list(pg_jieba_user_dict);

	/*
	 * Now that we've successfully built the new,
	 * save it in permanent storage.
	 */
	oldcxt = MemoryContextSwitchTo(TopMemoryContext);
	{
		new_dicts = pstrdup(user_dicts);

		/*
		 init will take a few seconds to load dicts.
		 */
		new_jieba = Jieba_New(dict_path, hmm_model_path, new_dicts);
	}
	MemoryContextSwitchTo(oldcxt);

	/* Now safe to assign to state variables. */
	if (userDicts)
		pfree(userDicts);
	userDicts = new_dicts;

	if (jieba) {
		Jieba_Free(jieba);
		jieba = NULL;
	}
	jieba = new_jieba;

	/* Mark the path valid. */
	userDictsValid = true;

	/* Clean up. */
	pfree(user_dicts);
	pfree(dict_path);
	pfree(hmm_model_path);
}

/* check_hook: validate new value */
static bool
check_user_dict(char **newval, void **extra, GucSource source)
{
	char	   *rawname;
	List	   *namelist;

	/* Need a modifiable copy of string */
	rawname = pstrdup(*newval);

	/* Parse string into list of identifiers */
	if (!SplitIdentifierString(rawname, ',', &namelist))
	{
		/* syntax error in name list */
		GUC_check_errdetail("List syntax is invalid.");
		pfree(rawname);
		list_free(namelist);
		return false;
	}

	/*
	 * We used to try to check that the named schemas exist, but there are
	 * many valid use-cases for having search_path settings that include
	 * schemas that don't exist; and often, we are not inside a transaction
	 * here and so can't consult the system catalogs anyway.  So now, the only
	 * requirement is syntactic validity of the identifier list.
	 */

	pfree(rawname);
	list_free(namelist);

	return true;
}

/* assign_hook: do extra actions as needed */
static void
assign_user_dict(const char *newval, void *extra)
{
	/*
	 * We mark the path as needing recomputation, but don't do anything until
	 * it's needed.  This avoids trying to do database access during GUC
	 * initialization, or outside a transaction.
	 */
//	userDictsValid = false;
}

/*
 * Given the base name and extension of a tsearch config file, return
 * its full path name.  The base name is assumed to be user-supplied,
 * and is checked to prevent pathname attacks.  The extension is assumed
 * to be safe.
 *
 * The result is a palloc'd string.
 */
static char*
jieba_get_tsearch_config_filename(const char *basename,
								  const char *extension)
{
	char		sharepath[MAXPGPATH];
	char	   *result;

	/*
	 * We limit the basename to contain a-z, 0-9, and underscores.  This may
	 * be overly restrictive, but we don't want to allow access to anything
	 * outside the tsearch_data directory, so for instance '/' *must* be
	 * rejected, and on some platforms '\' and ':' are risky as well. Allowing
	 * uppercase might result in incompatible behavior between case-sensitive
	 * and case-insensitive filesystems, and non-ASCII characters create other
	 * interesting risks, so on the whole a tight policy seems best.
	 */
	if (strspn(basename, "abcdefghijklmnopqrstuvwxyz0123456789_.") != strlen(basename))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("invalid text search configuration file name \"%s\"",
						basename)));

	get_share_path(my_exec_path, sharepath);
	result = palloc(MAXPGPATH);
	snprintf(result, MAXPGPATH, "%s/tsearch_data/%s.%s",
			 sharepath, basename, extension);

	return result;
}

/*
 * The result is a palloc'd string.
 */
static char *
extract_dict_list(const char *dictsString)
{
	List	   *elemlist;
	ListCell   *lc;
	char       *rawstring;

	bool		first;
	StringInfoData bufdata;
	StringInfo buf = &bufdata;

	initStringInfo(&bufdata);

	rawstring = pstrdup(dictsString);

	// Parse string into list of identifiers
	if (!SplitIdentifierString(rawstring, ',', &elemlist)) {
		// syntax error in list
		pfree(rawstring);
		list_free(elemlist);
		ereport(LOG,
				(errcode(ERRCODE_SYNTAX_ERROR),
				 errmsg("parameter must be a list of dictionary")));
		return NULL;
	}

	first = true;
	foreach(lc, elemlist)
	{
		const char *dict_name = (const char *) lfirst(lc);
		char* dict_path = jieba_get_tsearch_config_filename(dict_name, DICT_EXT);

		if (!first)
			appendStringInfoString(buf, ";");

		appendStringInfoString(buf, dict_path);

		pfree(dict_path);

		first = false;
	}

	list_free(elemlist);
	pfree(rawstring);

	return buf->data;
}
