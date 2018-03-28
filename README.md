# pg_jieba
[![Lang](https://img.shields.io/badge/Language-C%2FC%2B%2B-green.svg)]()
[![BSD](https://img.shields.io/badge/License-BSD-green.svg)]()
[![Extension](https://img.shields.io/badge/Extension-PostgreSQL-green.svg)]()

pg_jieba is a PostgreSQL extension for full-text search of Chinese.  
It implements by importing cppjieba.  

## NOTE
It should work with PostgreSQL > 9.x  
It is tested with PostgreSQL 9.6.3 on CentOS 7  

**The master branch require C++11(gcc4.8+), because the new version of cppjieba upgrade to C++11.**  
**If the OS compiler did not support C++11, please try old version of pg_jieba like v1.0.1**


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

2. **Init submodule**
  ```
  cd pg_jieba

  # initilized sub-project
  git submodule update --init --recursive
  ```

3. **Compile**
  
  ```
  cd pg_jieba

  mkdir build
  cd build

  cmake ..
  # If postgresql is installed customized, Try cmd as following  
  # cmake -DCMAKE_PREFIX_PATH=/PATH/TO/PGSQL_INSTALL_DIR ..
  # Ubuntu, To specify version of pg(missing: PostgreSQL_TYPE_INCLUDE_DIR)
  # cmake -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/10/server ..
  # In some OS like Ubuntu. Try cmd as following may solve the compiling problem 
  # cmake -DCMAKE_CXX_FLAGS="-Wall -std=c++11" ..

  make
  make install 
  # if got error when doing "make install"
  # try "sudo make install"
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
jieba.user.dict.utf8
  ```
  云计算
  韩玉鉴赏
  蓝翔 nz
  ```

operation
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
