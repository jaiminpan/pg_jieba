/*-------------------------------------------------------------------------
 *
 * pg_jieba.c
 *	  a text search parser for Chinese
 *
 * Author: Jaimin Pan <jaimin.pan@gmail.com>
 *
 * IDENTIFICATION
 *	  pg_jieba.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "cppjieba/Jieba.hpp"
#include "miscadmin.h"
#include "utils/guc.h"
#include "utils/builtins.h"


PG_MODULE_MAGIC;

/* Output token categories */

#define JB_A			1
#define JB_N			2
#define JB_V			3
#define JB_D			4

/* Start From 1 and LASTNUM is the last number */
#define LASTNUM			4

static const char *const tok_alias[] = {
	"",
	"a",
	"n",
	"v",
	"d"
};

static const char *const lex_descr[] = {
	"",
	"adjective",
	"noun",
	"verb",
	"adverb"
};

/*
 * types
 */

/* self-defined type */
typedef struct
{
	char	   *buffer;			/* text to parse */
	int			len;			/* length of the text in buffer */
	int			pos;			/* position of the parser */
	JiebaCtx   *ctx;
} ParserState;

/* copy-paste from wparser.h of tsearch2 */
typedef struct
{
	int			lexid;
	char	   *alias;
	char	   *descr;
} LexDescr;


void		_PG_init(void);
void		_PG_fini(void);

/*
 * prototypes
 */
PG_FUNCTION_INFO_V1(jieba_start);
Datum		jieba_start(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_gettoken);
Datum		jieba_gettoken(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_end);
Datum		jieba_end(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(jieba_lextype);
Datum		jieba_lextype(PG_FUNCTION_ARGS);


static JiebaCtx* jieba_ctx = NULL;
static const char* DICT_PATH = "libjieba/dict/jieba.dict";
static const char* HMM_PATH = "libjieba/dict/jieba.hmm_model";
static const char* USER_DICT = "libjieba/dict/jieba.user.dict";
static const char* EXT = "utf8";

static char *jieba_get_tsearch_config_filename(const char *basename, const char *extension);

/*
 * Module load callback
 */
void
_PG_init(void)
{
	if (jieba_ctx)
		return;

	{
		const char* dict_path = jieba_get_tsearch_config_filename(DICT_PATH, EXT);
		const char* hmm_path = jieba_get_tsearch_config_filename(HMM_PATH, EXT);
		const char* user_dict_path = jieba_get_tsearch_config_filename(USER_DICT, EXT);

	/*
	 init will take a few seconds to load dicts.
	 */
	jieba_ctx = Jieba_New(dict_path, hmm_path, user_dict_path);
	}
}

/*
 * Module unload callback
 */
void
_PG_fini(void)
{
	if (jieba_ctx)
		Jieba_Free(jieba_ctx);
	jieba_ctx = NULL;
}

/*
 * functions
 */
Datum
jieba_start(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));

	pst->buffer = (char *) PG_GETARG_POINTER(0);
	pst->len = PG_GETARG_INT32(1);
	pst->pos = 0;

	pst->ctx = jieba_ctx;

	Jieba_Cut(jieba_ctx, pst->buffer, pst->len);

	PG_RETURN_POINTER(pst);
}

Datum
jieba_gettoken(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);
	char	  **t = (char **) PG_GETARG_POINTER(1);
	int		   *tlen = (int *) PG_GETARG_POINTER(2);
	int			type = -1;

	JiebaResult* curr = Jieba_GetNext(pst->ctx);

	/* already done the work, or no sentence */
	if (curr == NULL)
	{
		*tlen = 0;
		type = 0;

		PG_RETURN_INT32(type);
	}

	// type = (int)(curr->attr)[0];
	type = JB_N;
	*tlen = curr->len;
	*t = curr->str;

	PG_RETURN_INT32(type);
}

Datum
jieba_end(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);

	pfree(pst);
	PG_RETURN_VOID();
}

Datum
jieba_lextype(PG_FUNCTION_ARGS)
{
	LexDescr   *descr = (LexDescr *) palloc(sizeof(LexDescr) * (LASTNUM + 1));
	int			i;

	for (i = 1; i <= LASTNUM; i++)
	{
		descr[i - 1].lexid = i;
		descr[i - 1].alias = pstrdup(tok_alias[i]);
		descr[i - 1].descr = pstrdup(lex_descr[i]);
	}

	descr[LASTNUM].lexid = 0;

	PG_RETURN_POINTER(descr);
}


/*
 * Given the base name and extension of a tsearch config file, return
 * its full path name.	The base name is assumed to be user-supplied,
 * and is checked to prevent pathname attacks.	The extension is assumed
 * to be safe.
 *
 * The result is a palloc'd string.
 */
static char *
jieba_get_tsearch_config_filename(const char *basename,
							const char *extension)
{
	char		sharepath[MAXPGPATH];
	char	   *result;

	/*
	 * We limit the basename to contain a-z, 0-9, and underscores.	This may
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

