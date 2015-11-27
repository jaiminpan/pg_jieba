/*-------------------------------------------------------------------------
 *
 * jieba.h
 *	  Contains functions which acted as an interface between the C++ and C
 *
 * Author: Jaimin Pan <jaimin.pan@gmail.com>
 *
 * IDENTIFICATION
 *	  libjieba/jieba.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef JIEBA_API_H
#define JIEBA_API_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct JiebaCtx JiebaCtx;

typedef struct JiebaResult
{
	char* str;
	unsigned char len;
	char attr[3];
} JiebaResult;

extern JiebaCtx* Jieba_New(const char* dict_path, const char* hmm_path, const char* user_dict);
extern void Jieba_Free(JiebaCtx* ctx);
extern void Jieba_Cut(JiebaCtx* ctx, const char* str, int len);
extern JiebaResult* Jieba_GetNext(JiebaCtx* ctx);


#ifdef __cplusplus
}
#endif

#endif
