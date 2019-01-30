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

#include "utils/guc.h"
#include "utils/elog.h"
#include "miscadmin.h"
#include "tsearch/ts_public.h"

#include "jieba.h"

PG_MODULE_MAGIC;

/* Start From 1 and LASTNUM is the last number */
int LASTNUM = sizeof(tok_alias) / sizeof(tok_alias[0]);

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

PG_FUNCTION_INFO_V1(jieba_gettoken);
Datum jieba_gettoken(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_end);
Datum jieba_end(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_lextype);
Datum jieba_lextype(PG_FUNCTION_ARGS);


static JiebaCtx* jieba = NULL;
static const char* DICT_PATH = "jieba.dict";
static const char* HMM_PATH = "jieba.hmm_model";
static const char* USER_DICT = "jieba.user.dict";
static const char* EXT = "utf8";

static char* jieba_get_tsearch_config_filename(const char *basename, const char *extension);

/*
 * Module load callback
 */
void
_PG_init(void)
{
	if (jieba) {
		return;
	}

	/*
	 init will take a few seconds to load dicts.
	 */
	jieba = Jieba_New(jieba_get_tsearch_config_filename(DICT_PATH, EXT),
					  jieba_get_tsearch_config_filename(HMM_PATH, EXT),
					  jieba_get_tsearch_config_filename(USER_DICT, EXT));
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

	pst->stat = Jieba_Cut(pst->ctx, pst->buffer, pst->len);

	PG_RETURN_POINTER(pst);
}

Datum
jieba_query_start(PG_FUNCTION_ARGS)
{
	ParserState* const pst = (ParserState *) palloc0(sizeof(ParserState));
	pst->buffer = (char *) PG_GETARG_POINTER(0);
	pst->len = PG_GETARG_INT32(1);

	pst->ctx = jieba;

	pst->stat = Jieba_CutForSearch(pst->ctx, pst->buffer, pst->len);

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
		descr[i - 1].alias = pstrdup(tok_alias[i]);
		descr[i - 1].descr = pstrdup(lex_descr[i]);
	}

	descr[size].lexid = 0;

	PG_RETURN_POINTER(descr);
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
