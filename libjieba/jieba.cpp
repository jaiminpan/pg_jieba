/*-------------------------------------------------------------------------
 *
 * jieba.cpp
 *	  Contains functions which acted as an interface between the C++ and C
 *
 * Author: Jaimin Pan <jaimin.pan@gmail.com>
 *
 * IDENTIFICATION
 *	  libjieba/jieba.cpp
 *
 *-------------------------------------------------------------------------
 */

#include "jieba.h"
#include "jieba/MixSegment.hpp"

using namespace cppjieba;

#ifdef __cplusplus
extern "C" {
#endif

struct JiebaCtx
{
	void *ctx;
	vector<string>::iterator iter;
	vector<string> *words;
	JiebaResult cur;
};

JiebaCtx *
Jieba_New(const char* dict_path, const char* hmm_path, const char* user_dict)
{
	JiebaCtx* ctx = new JiebaCtx();
	ctx->ctx = (void *)(new MixSegment(dict_path, hmm_path, user_dict));
	ctx->words = NULL;

	return ctx;
}

void
Jieba_Free(JiebaCtx* ctx)
{
	MixSegment* x = (MixSegment*) ctx->ctx;
	delete x;

	if (ctx->words)
		delete ctx->words;

	delete ctx;
}

void
Jieba_Cut(JiebaCtx* ctx, const char* str, int len)
{
	MixSegment* x = (MixSegment*) ctx->ctx;
	vector<string> *words = new vector<string>;
	string sentence(str, len);

	x->Cut(sentence, *words);

	if (ctx->words)
		delete ctx->words;

	ctx->words = words;
	ctx->iter = words->begin();
}

JiebaResult *
Jieba_GetNext(JiebaCtx* ctx)
{
	vector<string>::iterator& cur_iter = ctx->iter;
	vector<string>::iterator end_iter = ctx->words->end();

	JiebaResult* result = &(ctx->cur);

	if (!ctx->words || cur_iter == end_iter)
		return NULL;

	result->str = (char *) cur_iter->c_str();
	result->len = cur_iter->length();

	++cur_iter;

	return result;
}


#ifdef __cplusplus
}
#endif
