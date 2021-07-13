-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_jieba" to load this file. \quit

ALTER EXTENSION jieba ADD FUNCTION jieba_start(internal,integer);
ALTER EXTENSION jieba ADD FUNCTION jieba_query_start(internal,integer);
ALTER EXTENSION jieba ADD FUNCTION jieba_mp_start(internal,integer);
ALTER EXTENSION jieba ADD FUNCTION jieba_hmm_start(internal,integer);
ALTER EXTENSION jieba ADD FUNCTION jieba_gettoken(internal,internal,internal);
ALTER EXTENSION jieba ADD FUNCTION jieba_end(internal);
ALTER EXTENSION jieba ADD FUNCTION jieba_lextype(internal);
ALTER EXTENSION jieba ADD FUNCTION jieba_reload_dict(internal);
ALTER EXTENSION jieba ADD TEXT SEARCH PARSER jieba;
ALTER EXTENSION jieba ADD TEXT SEARCH PARSER jiebaqry;
ALTER EXTENSION jieba ADD TEXT SEARCH PARSER jiebahmm;
ALTER EXTENSION jieba ADD TEXT SEARCH PARSER jiebamp;
