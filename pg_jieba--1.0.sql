CREATE FUNCTION jieba_start(internal, integer)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION jieba_gettoken(internal, internal, internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION jieba_end(internal)
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION jieba_lextype(internal)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE TEXT SEARCH PARSER jieba (
	START    = jieba_start,
	GETTOKEN = jieba_gettoken,
	END      = jieba_end,
	LEXTYPES = jieba_lextype,
	HEADLINE = pg_catalog.prsd_headline
);

CREATE TEXT SEARCH CONFIGURATION jiebacfg (PARSER = jieba);

COMMENT ON TEXT SEARCH CONFIGURATION jiebacfg IS 'configuration for jieba';

CREATE TEXT SEARCH DICTIONARY jieba_stem (TEMPLATE=simple, stopwords = 'jieba');

COMMENT ON TEXT SEARCH DICTIONARY jieba_stem IS 'jieba dictionary: just lower case and check for stopword';

ALTER TEXT SEARCH CONFIGURATION jiebacfg ADD MAPPING FOR n,v,a,d WITH jieba_stem;
