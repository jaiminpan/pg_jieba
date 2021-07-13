CREATE FUNCTION jieba_start(internal, integer)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION jieba_query_start(internal, integer)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION jieba_mp_start(internal, integer)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE FUNCTION jieba_hmm_start(internal, integer)
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

CREATE FUNCTION jieba_reload_dict()
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;

CREATE TEXT SEARCH PARSER jieba (
	START    = jieba_start,
	GETTOKEN = jieba_gettoken,
	END      = jieba_end,
	LEXTYPES = jieba_lextype,
	HEADLINE = pg_catalog.prsd_headline
);

CREATE TEXT SEARCH PARSER jiebaqry (
	START    = jieba_query_start,
	GETTOKEN = jieba_gettoken,
	END      = jieba_end,
	LEXTYPES = jieba_lextype,
	HEADLINE = pg_catalog.prsd_headline
);

CREATE TEXT SEARCH PARSER jiebamp (
	START    = jieba_mp_start,
	GETTOKEN = jieba_gettoken,
	END      = jieba_end,
	LEXTYPES = jieba_lextype,
	HEADLINE = pg_catalog.prsd_headline
);

CREATE TEXT SEARCH PARSER jiebahmm (
	START    = jieba_hmm_start,
	GETTOKEN = jieba_gettoken,
	END      = jieba_end,
	LEXTYPES = jieba_lextype,
	HEADLINE = pg_catalog.prsd_headline
);

CREATE TEXT SEARCH CONFIGURATION jiebacfg (PARSER = jieba);
COMMENT ON TEXT SEARCH CONFIGURATION jiebacfg IS 'Mix segmentation configuration for jieba';

CREATE TEXT SEARCH CONFIGURATION jiebaqry (PARSER = jiebaqry);
COMMENT ON TEXT SEARCH CONFIGURATION jiebaqry IS 'Query segmentation configuration for jieba';

CREATE TEXT SEARCH CONFIGURATION jiebamp (PARSER = jiebamp);
COMMENT ON TEXT SEARCH CONFIGURATION jiebamp IS 'MP segmentation configuration for jieba';

CREATE TEXT SEARCH CONFIGURATION jiebahmm (PARSER = jiebahmm);
COMMENT ON TEXT SEARCH CONFIGURATION jiebahmm IS 'Hmm segmentation configuration for jieba';

CREATE TEXT SEARCH DICTIONARY jieba_stem (TEMPLATE=simple, stopwords = 'jieba');
COMMENT ON TEXT SEARCH DICTIONARY jieba_stem IS 'jieba dictionary: lower case and check for stopword which including Unicode symbols that are mainly Chinese characters and punctuations';

ALTER TEXT SEARCH CONFIGURATION jiebacfg ADD MAPPING FOR eng,nz,n,m,i,l,d,s,t,mq,nr,j,a,r,b,f,nrt,v,z,ns,q,vn,c,nt,u,o,zg,nrfg,df,p,g,y,ad,vg,ng,x,ul,k,ag,dg,rr,rg,an,vq,e,uv,tg,mg,ud,vi,vd,uj,uz,h,ug,rz WITH jieba_stem;
ALTER TEXT SEARCH CONFIGURATION jiebaqry ADD MAPPING FOR eng,nz,n,m,i,l,d,s,t,mq,nr,j,a,r,b,f,nrt,v,z,ns,q,vn,c,nt,u,o,zg,nrfg,df,p,g,y,ad,vg,ng,x,ul,k,ag,dg,rr,rg,an,vq,e,uv,tg,mg,ud,vi,vd,uj,uz,h,ug,rz WITH jieba_stem;
ALTER TEXT SEARCH CONFIGURATION jiebamp ADD MAPPING FOR eng,nz,n,m,i,l,d,s,t,mq,nr,j,a,r,b,f,nrt,v,z,ns,q,vn,c,nt,u,o,zg,nrfg,df,p,g,y,ad,vg,ng,x,ul,k,ag,dg,rr,rg,an,vq,e,uv,tg,mg,ud,vi,vd,uj,uz,h,ug,rz WITH jieba_stem;
ALTER TEXT SEARCH CONFIGURATION jiebahmm ADD MAPPING FOR eng,nz,n,m,i,l,d,s,t,mq,nr,j,a,r,b,f,nrt,v,z,ns,q,vn,c,nt,u,o,zg,nrfg,df,p,g,y,ad,vg,ng,x,ul,k,ag,dg,rr,rg,an,vq,e,uv,tg,mg,ud,vi,vd,uj,uz,h,ug,rz WITH jieba_stem;
