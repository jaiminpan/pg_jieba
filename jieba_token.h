/*-------------------------------------------------------------------------
 *
 * jieba_token.c
 *    token description
 *
 * Author: Jaimin Pan <jaimin.pan@gmail.com>
 *
 * IDENTIFICATION
 *    jieba_token.h
 *
 *-------------------------------------------------------------------------
 */

#ifndef JIEBA_TOKEN_H_
#define JIEBA_TOKEN_H_

typedef struct TokenDescData {
	const char* token;
	const char* descr;
} TokenDescData;

typedef struct TokenDescData *TokenDesc;

static const TokenDescData lex_descr[] =
{
  { .token = "",  .descr = ""},
  { .token = "eng",  .descr = "letter"},
  { .token = "nz",  .descr = "other proper noun"},
  { .token = "n",  .descr = "noun"},
  { .token = "m",  .descr = "numeral"},
  { .token = "i",  .descr = "idiom"},
  { .token = "l",  .descr = "temporary idiom"},
  { .token = "d",  .descr = "adverb"},
  { .token = "s",  .descr = "space"},
  { .token = "t",  .descr = "time"},
  { .token = "mq",  .descr = "numeral-classifier compound"},
  { .token = "nr",  .descr = "person's name"},
  { .token = "j",  .descr = "abbreviate"},
  { .token = "a",  .descr = "adjective"},
  { .token = "r",  .descr = "pronoun"},
  { .token = "b",  .descr = "difference"},
  { .token = "f",  .descr = "direction noun"},
  { .token = "nrt",  .descr = "nrt"},
  { .token = "v",  .descr = "verb"},
  { .token = "z",  .descr = "z"},
  { .token = "ns",  .descr = "location"},
  { .token = "q",  .descr = "quantity"},
  { .token = "vn",  .descr = "vn"},
  { .token = "c",  .descr = "conjunction"},
  { .token = "nt",  .descr = "organization"},
  { .token = "u",  .descr = "auxiliary"},
  { .token = "o",  .descr = "onomatopoeia"},
  { .token = "zg",  .descr = "zg"},
  { .token = "nrfg",  .descr = "nrfg"},
  { .token = "df",  .descr = "df"},
  { .token = "p",  .descr = "prepositional"},
  { .token = "g",  .descr = "morpheme"},
  { .token = "y",  .descr = "modal verbs"},
  { .token = "ad",  .descr = "ad"},
  { .token = "vg",  .descr = "vg"},
  { .token = "ng",  .descr = "ng"},
  { .token = "x",  .descr = "unknown"},
  { .token = "ul",  .descr = "ul"},
  { .token = "k",  .descr = "k"},
  { .token = "ag",  .descr = "ag"},
  { .token = "dg",  .descr = "dg"},
  { .token = "rr",  .descr = "rr"},
  { .token = "rg",  .descr = "rg"},
  { .token = "an",  .descr = "an"},
  { .token = "vq",  .descr = "vq"},
  { .token = "e",  .descr = "exclamation"},
  { .token = "uv",  .descr = "uv"},
  { .token = "tg",  .descr = "tg"},
  { .token = "mg",  .descr = "mg"},
  { .token = "ud",  .descr = "ud"},
  { .token = "vi",  .descr = "vi"},
  { .token = "vd",  .descr = "vd"},
  { .token = "uj",  .descr = "uj"},
  { .token = "uz",  .descr = "uz"},
  { .token = "h",  .descr = "h"},
  { .token = "ug",  .descr = "ug"},
  { .token = "rz",  .descr = "rz"}
};

/* Start From 1 and LASTNUM is the last number */
extern int LASTNUM;

#endif /* JIEBA_TOKEN_H_ */
