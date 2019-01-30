/*-------------------------------------------------------------------------
 *
 * jieba.h
 *	  Contains functions which acted as an interface between the C++ and C
 *
 * Author: Jaimin Pan <jaimin.pan@gmail.com>
 *
 * IDENTIFICATION
 *	  jieba.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef JIEBA_API_H
#define JIEBA_API_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct JiebaCtx JiebaCtx;
typedef struct ParStat ParStat;

typedef struct JiebaResult
{
	char* str;
	unsigned char len;
	int attr;
} JiebaResult;

extern JiebaCtx* Jieba_New(const char* dict_path, const char* hmm_path, const char* user_dict);
extern void Jieba_Free(JiebaCtx* ctx);
extern ParStat* Jieba_Cut(JiebaCtx* ctx, const char* str, int len);
extern ParStat* Jieba_CutForSearch(JiebaCtx* ctx, const char* str, int len);
extern JiebaResult* Jieba_GetNext(JiebaCtx* ctx, ParStat* stat);

extern ParStat* ParStat_New();
extern void ParStat_Free(ParStat* stat);

#include "jieba_token.h"

#ifdef __cplusplus
}
#endif

#endif /* JIEBA_API_H */
