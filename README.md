# pg_jieba
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


LICENSE
-------
The BSD License (BSD)  
Copyright (c) 2015-, Jaimin Pan (jaimin.pan@gmail.com)  
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of pg_jieba nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
