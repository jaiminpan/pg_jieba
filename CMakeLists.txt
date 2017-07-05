cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

set(CMAKE_COLOR_MAKEFILE ON)

set(PG_JIEBA_PROJECT_NAME "pg_jieba")
set(PG_JIEBA_PROJECT_ID "pg_jieba")
project("${PG_JIEBA_PROJECT_ID}")

include(CTest)
enable_testing()

message(STATUS "Setting ${CMAKE_PROJECT_NAME} build type - ${CMAKE_BUILD_TYPE}")

find_package(PostgreSQL REQUIRED)
if(NOT PostgreSQL_FOUND)
    message(FATAL_ERROR " Please check your PostgreSQL installation.")
endif(NOT PostgreSQL_FOUND)

include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${POSTGRESQL_LIBRARY_DIRS})

set(CPPJIEBA_DIR "libjieba")
INCLUDE_DIRECTORIES(${CPPJIEBA_DIR}/deps
        ${CPPJIEBA_DIR}/include)

set(SOURCE_FILES
    pg_jieba.c)

set(PG_JIEBA_POSTGRESQL_DIR "${CMAKE_INSTALL_PREFIX}"
        CACHE PATH "PostgreSQL binary directory")
set(PG_JIEBA_POSTGRESQL_VERSION "unknown"
        CACHE STRING "PostgreSQL version")

set(PG_JIEBA_DLL_NAME "pg_jieba")
set(PG_JIEBA_LIBRARY_NAME "lib${PG_JIEBA_DLL_NAME}")
set(PG_JIEBA_EXTENSION_DIR "lib")
set(PG_JIEBA_EXTENSION_DATA_DIR "share/extension")

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/${PG_JIEBA_PROJECT_ID}.control"
        PG_JIEBA_CONTROL)

string(REGEX REPLACE "^default_version = '([0-9.]+)'.*" "\\1"
        PG_JIEBA_VERSION "${PG_JIEBA_CONTROL}")
string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1"
        PG_JIEBA_VERSION_MAJOR "${PG_JIEBA_VERSION}")
string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2"
        PG_JIEBA_VERSION_MINOR "${PG_JIEBA_VERSION}")
string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3"
        PG_JIEBA_VERSION_MICRO "${PG_JIEBA_VERSION}")

add_library("${PG_JIEBA_LIBRARY_NAME}" SHARED ${SOURCE_FILES})
set_target_properties("${PG_JIEBA_LIBRARY_NAME}"
        PROPERTIES
        OUTPUT_NAME "${PG_JIEBA_DLL_NAME}")
target_link_libraries("${PG_JIEBA_LIBRARY_NAME}"
        "postgres.lib")

install(TARGETS "${PG_JIEBA_LIBRARY_NAME}"
        DESTINATION "${PG_JIEBA_EXTENSION_DIR}")
install(FILES
        "${PROJECT_SOURCE_DIR}/${PG_JIEBA_PROJECT_ID}.control"
        DESTINATION "${PG_JIEBA_EXTENSION_DATA_DIR}")
install(FILES
        "${PROJECT_SOURCE_DIR}/data/${PG_JIEBA_PROJECT_ID}.sql"
        RENAME "${PG_JIEBA_PROJECT_ID}--${PG_JIEBA_VERSION}.sql"
        DESTINATION "${PG_JIEBA_EXTENSION_DATA_DIR}")
file(GLOB PG_JIEBA_UPGRADE_SQL_FILES "data/${PG_JIEBA_PROJECT_ID}--*--*.sql")
install(FILES
        ${PG_JIEBA_UPGRADE_SQL_FILES}
        DESTINATION "${PG_JIEBA_EXTENSION_DATA_DIR}")
