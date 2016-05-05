# pg_jieba
[![BSD](https://img.shields.io/badge/License-BSD-green.svg)]()
[![Lang](https://img.shields.io/badge/Language-C%2FC%2B%2B-green.svg)]()

pg_jieba is a PostgreSQL extension for full-text search of Chinese.  
It implements by importing cppjieba.  

## NOTE
It should work with PostgreSQL > 9.x  
Now only tested with PostgreSQL 9.4  

PREPARE
-------
Make sure PostgreSQL is installed and command `pg_config` could be runnable.   
  
Install Postgres:  

* Option:  
  download from http://www.postgresql.org/download/

* Option:  
  CentOS: 

  ```
  sudo yum install postgresql postgresql-server postgresql-devel
  ```
* Option:  
  Download From http://www.enterprisedb.com/products-services-training/pgdownload


INSTALL
-------

1. **Downloads**

  ```
  git clone https://github.com/jaiminpan/pg_jieba
  ```
2. **Compile**
  
  ```
  cd pg_jieba
  USE_PGXS=1 make
  USE_PGXS=1 make install 
  # if got error when doing "USE_PGXS=1 make install"
  # try "sudo USE_PGXS=1 make install"
  ```

HOW TO USE & EXAMPLE
-------

  ```
  jieba=# create extension pg_jieba;
  CREATE EXTENSION

  jieba=#  select * from to_tsvector('jiebacfg', '小明硕士毕业于中国科学院计算所，后在日本京都大学深造');
                                                   to_tsvector
  --------------------------------------------------------------------------------------------------------------
   '中国科学院':5 '于':4 '后':8 '在':9 '小明':1 '日本京都大学':10 '毕业':3 '深造':11 '硕士':2 '计算所':6 '，':7
  (1 row)

  jieba=#  select * from to_tsvector('jiebacfg', '李小福是创新办主任也是云计算方面的专家');
                                          to_tsvector
  -------------------------------------------------------------------------------------------
   '专家':11 '主任':5 '也':6 '云计算':8 '创新':3 '办':4 '方面':9 '是':2,7 '李小福':1 '的':10
  (1 row)
  ```

## USER DEFINED DICTIONARY
Reference [dictionary link](https://github.com/jaiminpan/pg_jieba/tree/master/dict/jieba.user.dict.utf8
) for checking format

  ```
  cd /PATH/TO/POSTGRESQL_INSTALL/share/postgresql/tsearch_data
  OR
  cd /PATH/TO/POSTGRESQL_INSTALL/share/tsearch_data

  cp 'YOUR DICTIONARY' jieba.user.dict.utf8
  ```

## ONLINE TEST
You can test for result by following link (Suggest opened by Chrome)
http://cppjieba-webdemo.herokuapp.com/

## HISTORY
https://github.com/jaiminpan/pg_jieba/blob/master/HISTORY

## THANKS

jieba project by SunJunyi
https://github.com/fxsjy/jieba

cppjieba project by WuYanyi
https://github.com/yanyiwu/cppjieba

> Author: jaimin.pan@gmail.com    
