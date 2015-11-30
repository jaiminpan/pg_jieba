# contrib/pg_jieba/Makefile

MODULE_big = pg_jieba
OBJS = pg_jieba.o \
		jieba.o

EXTENSION = pg_jieba
DATA = pg_jieba--1.0.sql pg_jieba--unpackaged--1.0.sql
DATA_TSEARCH = dict/jieba.dict.utf8 dict/jieba.hmm_model.utf8 dict/jieba.user.dict.utf8

REGRESS = pg_jieba

SHLIB_LINK = -lstdc++

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/pg_jieba
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif

CFLAGS_JB := $(filter-out -Wmissing-prototypes -Wdeclaration-after-statement -fexcess-precision=standard, $(CFLAGS))

jieba.o: libjieba/jieba.cpp
	g++ $(CFLAGS_JB) $(CPPFLAGS) -DLOGGER_LEVEL=LL_WARN -o $@ -c $^
