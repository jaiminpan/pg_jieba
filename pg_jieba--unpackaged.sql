-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_jieba" to load this file. \quit

ALTER EXTENSION jieba ADD FUNCTION jieba_start(internal,integer);
ALTER EXTENSION jieba ADD FUNCTION jieba_query_start(internal,integer);
ALTER EXTENSION jieba ADD FUNCTION jieba_gettoken(internal,internal,internal);
ALTER EXTENSION jieba ADD FUNCTION jieba_end(internal);
ALTER EXTENSION jieba ADD FUNCTION jieba_lextype(internal);
ALTER EXTENSION jieba ADD TEXT SEARCH PARSER jieba;
