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

static const char* const tok_alias[] = {
  "",
  "nz",
  "n",
  "m",
  "i",
  "l",
  "d",
  "s",
  "t",
  "mq",
  "nr",
  "j",
  "a",
  "r",
  "b",
  "f",
  "nrt",
  "v",
  "z",
  "ns",
  "q",
  "vn",
  "c",
  "nt",
  "u",
  "o",
  "zg",
  "nrfg",
  "df",
  "p",
  "g",
  "y",
  "ad",
  "vg",
  "ng",
  "x",
  "ul",
  "k",
  "ag",
  "dg",
  "rr",
  "rg",
  "an",
  "vq",
  "e",
  "uv",
  "tg",
  "mg",
  "ud",
  "vi",
  "vd",
  "uj",
  "uz",
  "h",
  "ug",
  "rz"
};

static const char* const lex_descr[] = {
  "",
  "other proper noun",
  "noun",
  "numeral",
  "idiom",
  "temporary idiom",
  "adverb",
  "space",
  "time",
  "numeral-classifier compound",
  "person's name",
  "abbreviate",
  "adjective",
  "pronoun",
  "difference",
  "direction noun",
  "nrt",
  "verb",
  "z",
  "location",
  "quantity",
  "vn",
  "conjunction",
  "organization",
  "auxiliary",
  "onomatopoeia",
  "zg",
  "nrfg",
  "df",
  "prepositional",
  "morpheme",
  "modal verbs",
  "ad",
  "vg",
  "ng",
  "unknown",
  "ul",
  "k",
  "ag",
  "dg",
  "rr",
  "rg",
  "an",
  "vq",
  "exclamation",
  "uv",
  "tg",
  "mg",
  "ud",
  "vi",
  "vd",
  "uj",
  "uz",
  "h",
  "ug",
  "rz"
};

/* Start From 1 and LASTNUM is the last number */
extern int LASTNUM;

#endif /* JIEBA_TOKEN_H_ */
