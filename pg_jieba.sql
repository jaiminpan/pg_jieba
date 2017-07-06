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

ALTER TEXT SEARCH CONFIGURATION jiebacfg ADD MAPPING FOR nz,n,m,i,l,d,s,t,mq,nr,j,a,r,b,f,nrt,v,z,ns,q,vn,c,nt,u,o,zg,nrfg,df,p,g,y,ad,vg,ng,x,ul,k,ag,dg,rr,rg,an,vq,e,uv,tg,mg,ud,vi,vd,uj,uz,h,ug,rz WITH jieba_stem;
