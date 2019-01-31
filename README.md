# pg_jieba
[![Lang](https://img.shields.io/badge/Language-C%2FC%2B%2B-green.svg)]()
[![BSD](https://img.shields.io/badge/License-BSD-green.svg)]()
[![Extension](https://img.shields.io/badge/PostgreSQL_9.x-Extension-green.svg)]()
[![Extension](https://img.shields.io/badge/PostgreSQL_10.x-Extension-green.svg)]()
[![Extension](https://img.shields.io/badge/PostgreSQL_11.x-Extension-green.svg)]()

pg_jieba is a PostgreSQL extension for full-text search of Chinese.  

## NOTE
It is tested on [![Extension](https://img.shields.io/badge/PostgreSQL9.6.3-CentOS_7-green.svg)]() [![Extension](https://img.shields.io/badge/PostgreSQL11.1-MacOS_Mojave-green.svg)]()  

**This branch require C++11(gcc4.8+), because the new version of cppjieba upgrade to C++11.**  
**If the OS compiler did not support C++11, please try old version of pg_jieba as branch v1.0.1**


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

#### 1. Downloads

  ```
  git clone https://github.com/jaiminpan/pg_jieba
  ```

#### 2. Init submodule
  ```
  cd pg_jieba

  # initilized sub-project
  git submodule update --init --recursive
  ```

#### 3. Compile

  ```sh
  cd pg_jieba

  mkdir build
  cd build

  cmake ..
  
  make
  make install 
  # if got error when doing "make install"
  # try "sudo make install"
  ```
  ##### Compile Failed Q&A
  Q: Postgresql is installed customized  
  A: Try cmd as following  
     `cmake -DCMAKE_PREFIX_PATH=/PATH/TO/PGSQL_INSTALL_DIR ..`

  Q: Ubuntu, To specify version of pg(missing: PostgreSQL_TYPE_INCLUDE_DIR)  
  A: `cmake -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/10/server ..`  

  Q: In some OS such as Ubuntu.  
  A: Try cmd as following  
     `cmake -DCMAKE_CXX_FLAGS="-Wall -std=c++11" ..`


HOW TO USE & EXAMPLE
-------

Here is alternative configs;
* **jiebacfg:** Used in most situation (Recommand)
* **jiebaqry:** Similar to the one used by web search engines.

```sh
jieba=# select * from to_tsquery('jiebacfg', '我来到北京清华大学');
          to_tsquery
------------------------------
 '来到' & '北京' & '清华大学'
(1 row)

jieba=# select * from to_tsquery('jiebaqry', '我来到北京清华大学');
                       to_tsquery
---------------------------------------------------------
 '来到' & '北京' & '清华' & '华大' & '大学' & '清华大学'
(1 row)
```

#### General
  ```sh
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

  jieba=# select * from to_tsvector('jiebacfg', '是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。');
                                            to_tsvector
-----------------------------------------------------------------------------------------------------
 '不用':8 '专业':5 '会':13 '加薪':15 '升职':14 '多久':9 '学院':3 '当上':17 '手扶拖拉机':4 '拖拉机':2
(1 row)
  ```

#### Token And Tag
```sh
jieba=# select * from ts_token_type('jieba');
 tokid | alias |         description
-------+-------+-----------------------------
     1 | nz    | other proper noun
     2 | n     | noun
     3 | m     | numeral
... ...
... ...
    54 | ug    | ug
    55 | rz    | rz
    56 |       |
(56 rows)

jieba=# select * from ts_debug('jiebacfg', '是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。');
 alias |  description  |   token    | dictionaries | dictionary |   lexemes
-------+---------------+------------+--------------+------------+--------------
 v     | verb          | 是         | {jieba_stem} | jieba_stem | {}
 n     | noun          | 拖拉机     | {jieba_stem} | jieba_stem | {拖拉机}
 n     | noun          | 学院       | {jieba_stem} | jieba_stem | {学院}
 n     | noun          | 手扶拖拉机 | {jieba_stem} | jieba_stem | {手扶拖拉机}
 n     | noun          | 专业       | {jieba_stem} | jieba_stem | {专业}
 uj    | uj            | 的         | {jieba_stem} | jieba_stem | {}
 x     | unknown       | 。         | {jieba_stem} | jieba_stem | {}
 v     | verb          | 不用       | {jieba_stem} | jieba_stem | {不用}
 m     | numeral       | 多久       | {jieba_stem} | jieba_stem | {多久}
 x     | unknown       | ，         | {jieba_stem} | jieba_stem | {}
 r     | pronoun       | 我         | {jieba_stem} | jieba_stem | {}
 d     | adverb        | 就         | {jieba_stem} | jieba_stem | {}
 v     | verb          | 会         | {jieba_stem} | jieba_stem | {会}
 v     | verb          | 升职       | {jieba_stem} | jieba_stem | {升职}
 nr    | person's name | 加薪       | {jieba_stem} | jieba_stem | {加薪}
 x     | unknown       | ，         | {jieba_stem} | jieba_stem | {}
 t     | time          | 当上       | {jieba_stem} | jieba_stem | {当上}
(17 rows)
```

## USER DEFINED DICTIONARY
#### Dictionary Format
+ Words  weight  type
+ Words  type
+ Words
  ```
  云计算
  韩玉鉴赏
  蓝翔 nz
  区块链 10 nz
  ```
Reference `jieba_user.dict`

#### How to use your own dictionary
  ```
  cd /PATH/TO/POSTGRESQL_INSTALL/share/postgresql/tsearch_data
  OR
  cd /PATH/TO/POSTGRESQL_INSTALL/share/tsearch_data

  cp 'YOUR DICTIONARY' jieba_user.dict
  ```

#### Dictionary Sharing

+ [dict.367W.utf8] iLife


## Parameter
The following configuration options are available to add into postgresql.conf

+ **pg_jieba.hmm_model** (Need Restart)
HMM Model file.
+ **pg_jieba.base_dict** (Need Restart)
Base dictionary.
+ **pg_jieba.user_dict** (Need Restart)
csv list of specific user dictionary name(Exclude suffix `.dict`).
All should located in dir `tsearch_data`.

## Online Test
You can test for result by [test link] (Suggest opened by Chrome)


## HISTORY
[history]

## Package Dependency
  
* cppjieba v5.0.0

## THANKS

[jieba] project by SunJunyi
[CppJieba] project by WuYanyi

[history]:https://github.com/jaiminpan/pg_jieba/blob/master/HISTORY
[test link]:http://cppjieba-webdemo.herokuapp.com/
[dict.367W.utf8]:https://github.com/qinwf/BigDict
[CppJieba]:https://github.com/yanyiwu/cppjieba
[jieba]:https://github.com/fxsjy/jieba
