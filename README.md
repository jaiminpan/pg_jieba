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

#### General
  ```sh
  jieba=# create extension pg_jieba;
  CREATE EXTENSION

  jieba=# select * from to_tsquery('jiebacfg', '是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。');
                                          to_tsquery
-----------------------------------------------------------------------------------------------
 '拖拉机' & '学院' & '手扶拖拉机' & '专业' & '不用' & '多久' & '会' & '升职' & '加薪' & '当上' & 'ceo' & '走上' & '人生' & '巅峰'
(1 row)

  jieba=# select * from to_tsvector('jiebacfg', '是拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上CEO，走上人生巅峰。');
                                            to_tsvector
-----------------------------------------------------------------------------------------------------
 'ceo':18 '不用':8 '专业':5 '人生':21 '会':13 '加薪':15 '升职':14 '多久':9 '学院':3 '巅峰':22 '当上':17 '手扶拖拉机':4 '拖拉机':2 '走上':20
(1 row)
  ```

#### Token And Tag
```sh
jieba=# select * from ts_token_type('jieba');
 tokid | alias |         description
-------+-------+-----------------------------
     1 | eng   | letter
     2 | nz    | other proper noun
     3 | n     | noun
... ...
... ...
    55 | ug    | ug
    56 | rz    | rz
    57 |       |
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
 eng   | letter        | CEO        | {jieba_stem} | jieba_stem | {ceo}
 x     | unknown       | ，         | {jieba_stem} | jieba_stem | {}
 v     | verb          | 走上       | {jieba_stem} | jieba_stem | {走上}
 n     | noun          | 人生       | {jieba_stem} | jieba_stem | {人生}
 n     | noun          | 巅峰       | {jieba_stem} | jieba_stem | {巅峰}
 x     | unknown       | 。         | {jieba_stem} | jieba_stem | {}
```

#### Here is alternative configs;
* **jiebamp:** Use mp
* **jiebahmm:** Use hmm
* **jiebacfg:** Combine MP&HMM(Mix). Used in most situation (Recommand)
* **jiebaqry:** First use Mix, then use full. Similar to the one used by web search engines.

| Config   | Statment                                             | Result                                                       |
| -------- | ---------------------------------------------------- | ------------------------------------------------------------ |
| jiebamp  | 我来到北京清华大学                                   | '来到' & '北京' & '清华大学'                                 |
| jiebamp  | 他来到了网易杭研大厦                                 | '来到' & '网易' & '杭' & '研' & '大厦'                       |
| jiebamp  | 小明硕士毕业于中国科学院计算所，后在日本京都大学深造 | '明' & '硕士' & '毕业' & '中国科学院' & '计算所' & '日本京都大学' & '深造' |

| Config   | Statment                                             | Result                                                       |
| -------- | ---------------------------------------------------- | ------------------------------------------------------------ |
| jiebahmm | 我来到北京清华大学                                   | '我来' & '北京' & '清华大学'                                 |
| jiebahmm | 他来到了网易杭研大厦                                 | '他来' & '网易' & '杭' & '研大厦'                            |
| jiebahmm | 小明硕士毕业于中国科学院计算所，后在日本京都大学深造 | '小明' & '硕士' & '毕业于' & '中国' & '科学院' & '计算' & '日' & '本京' & '大学' & '深造' |

| Config   | Statment                                             | Result                                                       |
| -------- | ---------------------------------------------------- | ------------------------------------------------------------ |
| jiebacfg | 我来到北京清华大学                                   | '来到' & '北京' & '清华大学'                                 |
| jiebacfg | 他来到了网易杭研大厦                                 | '来到' & '网易' & '杭研' & '大厦'                            |
| jiebacfg | 小明硕士毕业于中国科学院计算所，后在日本京都大学深造 | '小明' & '硕士' & '毕业' & '中国科学院' & '计算所' & '日本京都大学' & '深造' |

| Config   | Statment                                             | Result                                                       |
| -------- | ---------------------------------------------------- | ------------------------------------------------------------ |
| jiebaqry | 我来到北京清华大学                                   | '来到' & '北京' & '清华' & '华大' & '大学' & '清华大学'      |
| jiebaqry | 他来到了网易杭研大厦                                 | '来到' & '网易' & '杭研' & '大厦'                            |
| jiebaqry | 小明硕士毕业于中国科学院计算所，后在日本京都大学深造 | '小明' & '硕士' & '毕业' & '中国' & '科学' & '学院' & '科学院' & '中国科学院' & '计算' & '计算所' & '日本' & '京都' & '大学' & '日本京都大学' & '深造' |


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
When `pg_jieba` loaded by `shared_preload_libraries`,
The following configuration options are available and can be added into postgresql.conf

+ **pg_jieba.hmm_model** (Need Restart)
  HMM Model file.
+ **pg_jieba.base_dict** (Need Restart)
  Base dictionary.
+ **pg_jieba.user_dict** (Need Restart)
  csv list of specific user dictionary name(Exclude suffix `.dict`).
  All should located in dir `tsearch_data`.

#### Postgresql parameter
```sh
# shared_preload_libraries = 'pg_jieba.so'  # (change requires restart)

# default_text_search_config='pg_catalog.simple'; default value
# default_text_search_config='jiebacfg'; uncomment to make 'jiebacfg' as default
```

## Online Test
You can test for result by [test link] (Suggest opened by Chrome)


## HISTORY
[history]

## Package Dependency

* cppjieba v5.1

## Docker
There is [docker file] by @ssfdust.
```sh
# scripts
docker run --name testjieba -e POSTGRES_PASSWORD=passwd -e POSTGRES_USER=test -e POSTGRES_DB=testdb -d ssfdust/psql_jieba_swsc
docker exec -ti testjieba psql -U test testdb
```

## THANKS

[jieba] project by SunJunyi  
[CppJieba] project by WuYanyi  

[history]:https://github.com/jaiminpan/pg_jieba/blob/master/HISTORY
[test link]:http://cppjieba-webdemo.herokuapp.com/
[dict.367W.utf8]:https://github.com/qinwf/BigDict
[CppJieba]:https://github.com/yanyiwu/cppjieba
[jieba]:https://github.com/fxsjy/jieba
[docker file]:https://cloud.docker.com/repository/docker/ssfdust/psql_jieba_swsc/general
