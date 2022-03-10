# 指定CMake版本
cmake_minimum_required(VERSION 3.5)
 
# 指定项目名称
project(test)
 
# 判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCC)
    message("COMPILER IS GNUCC")
    add_definitions ( -std=c++11 )
endif(CMAKE_COMPILER_IS_GNUCC)
 
#使用Debug编译模式
SET(CMAKE_BUILD_TYPE "Debug")
 
# C++ 调试编译时使用的标志
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -ggdb3")
# C++ 发行编译时使用的标志
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(HOME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../base)
set(DB_PROXY_SERVER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../db_proxy_server)

message("BASE_DIR:${BASE_DIR}")
 
# 指定头文件目录
include_directories(${BASE_DIR} ${BASE_DIR}/SLog)
include_directories(${BASE_DIR}/Config)
include_directories(${BASE_DIR}/UtilPdu)
include_directories(${BASE_DIR}/EncDec)
include_directories(${BASE_DIR}/Lock)
include_directories(${BASE_DIR}/Util)
include_directories(${BASE_DIR}/ThreadPool)
include_directories(${BASE_DIR}/ImPduBase)
include_directories(${BASE_DIR}/NetLib)
include_directories(${BASE_DIR}/HttpParser)
include_directories(${BASE_DIR}/ImConn)
include_directories(${BASE_DIR}/HttpClient)
include_directories(${DB_PROXY_SERVER_DIR}/CachePool)
include_directories(${DB_PROXY_SERVER_DIR}/DbPool)
include_directories(${DB_PROXY_SERVER_DIR}/AudioModel)
include_directories(${DB_PROXY_SERVER_DIR}/DepartModel)
include_directories(${DB_PROXY_SERVER_DIR}/UserModel)


# ImPduBase 依赖 protobuf
SET(PB_LIB_DIR ${HOME_DIR}/3rdParty/pb/lib/linux)
include_directories(${HOME_DIR}/3rdParty ${HOME_DIR}/3rdParty/pb ${HOME_DIR}/3rdParty/pb/protocol)

# HttpClient 依赖 jsoncpp
SET(JSON_LIB_DIR ${HOME_DIR}/3rdParty/jsoncpp)
include_directories(${HOME_DIR}/3rdParty/jsoncpp)

# DbPool 依赖 mysql
SET(MYSQL_INCLUDE_DIR /usr/include/mysql)
include_directories(${MYSQL_INCLUDE_DIR})
SET(MYSQL_LIB /usr/lib64/mysql)


# 指定链接库路径和链接库等并生成可执行文件
# SLog 这里直接使用了自己生成的libslog.so而libslog.so依赖了第三方开源库log4cxx
link_directories(${BASE_DIR}/SLog)
link_libraries(slog)
add_executable(test_slog ${HOME_DIR}/Tests/test_slog.cpp)

# Config
file(GLOB_RECURSE CONFIG_SRCS ${BASE_DIR}/Config/*.cpp)
add_executable(test_config ${CONFIG_SRCS} ${HOME_DIR}/Tests/test_config.cpp)

# UtilPdu
file(GLOB_RECURSE UTILPDU_SRCS ${BASE_DIR}/UtilPdu/*.cpp)
add_executable(test_utilpdu ${UTILPDU_SRCS} ${HOME_DIR}/Tests/test_utilpdu.cpp)

# EncDec 依赖了 UtilPdu 依赖系统库 openssl/aes openssl/md5
file(GLOB_RECURSE ENCDEC_SRCS ${BASE_DIR}/EncDec/*.cpp)
add_executable(test_encdec ${UTILPDU_SRCS} ${ENCDEC_SRCS} ${HOME_DIR}/Tests/test_encdec.cpp)
target_link_libraries(test_encdec crypto)

# Lock 是pthread.h实现的依赖系统库pthread
file(GLOB_RECURSE LOCK_SRCS ${BASE_DIR}/Lock/*.cpp)

# Util 依赖了 Lock SLog
file(GLOB_RECURSE UTIL_SRCS ${BASE_DIR}/Util/*.cpp)
add_executable(test_util ${LOCK_SRCS} ${UTIL_SRCS} ${HOME_DIR}/Tests/test_util.cpp)
target_link_libraries(test_util pthread slog)

# ThreadPool 依赖了 Util 依赖系统库 pthread
file(GLOB_RECURSE THREADPOOL_SRCS ${BASE_DIR}/ThreadPool/*.cpp)
add_executable(test_threadpool ${LOCK_SRCS} ${UTIL_SRCS} ${THREADPOOL_SRCS} ${HOME_DIR}/Tests/test_threadpool.cpp)
target_link_libraries(test_threadpool pthread slog)

# Lock 是pthread.h实现的依赖系统库pthread test_lock依赖了threadpool
add_executable(test_lock ${LOCK_SRCS} ${UTIL_SRCS} ${THREADPOOL_SRCS} ${HOME_DIR}/Tests/test_lock.cpp)
target_link_libraries(test_lock pthread slog)


# ImPduBase 依赖了 UtilPdu Util 依赖第三方开源库 protobuf
aux_source_directory(${HOME_DIR}/3rdParty/pb/protocol SRC_PB)
link_directories(${PB_LIB_DIR})
file(GLOB_RECURSE IMPDUBASE_SRCS ${BASE_DIR}/ImPduBase/*.cpp)
add_executable(test_impdubase ${UTILPDU_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${SRC_PB} ${IMPDUBASE_SRCS} ${HOME_DIR}/Tests/test_impdubase.cpp)
target_link_libraries(test_impdubase pthread protobuf-lite slog)


# NetLib 依赖了 Util
file(GLOB_RECURSE NETLIB_SRCS ${BASE_DIR}/NetLib/*.cpp)
add_executable(test_netlib ${LOCK_SRCS} ${UTIL_SRCS} ${NETLIB_SRCS} ${HOME_DIR}/Tests/test_netlib.cpp)
target_link_libraries(test_netlib pthread slog)

# HttpParser
file(GLOB_RECURSE HTTPPARSER_SRCS ${BASE_DIR}/HttpParser/*.cpp)
add_executable(test_httpparser ${HTTPPARSER_SRCS} ${HOME_DIR}/Tests/test_httpparser.cpp)

# ImConn 依赖了 ImPduBase NetLib 
file(GLOB_RECURSE IMCONN_SRCS ${BASE_DIR}/ImConn/*.cpp)
add_executable(test_imconn ${LOCK_SRCS} ${UTIL_SRCS} ${UTILPDU_SRCS} ${IMPDUBASE_SRCS} ${SRC_PB} ${LOCK_SRCS} ${NETLIB_SRCS} ${IMCONN_SRCS} ${HOME_DIR}/Tests/test_imconn.cpp)
target_link_libraries(test_imconn pthread protobuf-lite slog)

# HttpClient 依赖第三方库 curl jsoncpp
aux_source_directory(${JSON_LIB_DIR} SRC_JSON)
file(GLOB_RECURSE HTTPCLIENT_SRCS ${BASE_DIR}/HttpClient/*.cpp)
add_executable(test_httpclient ${SRC_JSON} ${HTTPCLIENT_SRCS} ${HOME_DIR}/Tests/test_httpclient.cpp)
target_link_libraries(test_httpclient curl)

# CachePool 依赖了 Config Util ThreadPool 使用了redis接口，依赖hiredis
file(GLOB_RECURSE CACHEPOOL_SRCS ${DB_PROXY_SERVER_DIR}/CachePool/*.cpp)
add_executable(test_cachepool  ${CONFIG_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${THREADPOOL_SRCS} ${CACHEPOOL_SRCS} ${HOME_DIR}/Tests/test_cachepool.cpp)
target_link_libraries(test_cachepool pthread hiredis slog)

# DbPool 依赖了 Config Lock Util ThreadPool 使用了mysql接口，依赖mysqlclient
file(GLOB_RECURSE DBPOOL_SRCS ${DB_PROXY_SERVER_DIR}/DbPool/*.cpp)
link_directories(${MYSQL_LIB})
add_executable(test_dbpool  ${CONFIG_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${THREADPOOL_SRCS} ${DBPOOL_SRCS} ${HOME_DIR}/Tests/test_dbpool.cpp)
target_link_libraries(test_dbpool pthread mysqlclient slog)

# AudioModel 依赖了 UtilPdu DbPool HttpClient 依赖第三方开源库 protobuf
file(GLOB_RECURSE AUDIOMODEL_SRCS ${DB_PROXY_SERVER_DIR}/AudioModel/*.cpp)
add_executable(test_audiomodel ${UTILPDU_SRCS} ${CONFIG_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${THREADPOOL_SRCS} ${DBPOOL_SRCS} ${SRC_JSON} ${HTTPCLIENT_SRCS} ${SRC_PB} ${AUDIOMODEL_SRCS} ${HOME_DIR}/Tests/test_audiomodel.cpp)
target_link_libraries(test_audiomodel pthread mysqlclient curl protobuf-lite slog)

# DepartModel 依赖了 ImPduBase DbPool 依赖第三方开源库 protobuf
file(GLOB_RECURSE DEPARTMODEL_SRCS ${DB_PROXY_SERVER_DIR}/DepartModel/*.cpp)
add_executable(test_departmodel ${UTILPDU_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${SRC_PB} ${CONFIG_SRCS} ${THREADPOOL_SRCS} ${DBPOOL_SRCS} ${IMPDUBASE_SRCS} ${DEPARTMODEL_SRCS} ${HOME_DIR}/Tests/test_departmodel.cpp)
target_link_libraries(test_departmodel pthread protobuf-lite slog mysqlclient)


# UserModel 依赖了 ImPduBase DbPool CachePool 依赖第三方开源库 protobuf
file(GLOB_RECURSE USERMODEL_SRCS ${DB_PROXY_SERVER_DIR}/UserModel/*.cpp)
message("SRC_PB:${SRC_PB}")
add_executable(test_usermodel ${UTILPDU_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${SRC_PB} ${CONFIG_SRCS} ${THREADPOOL_SRCS} ${DBPOOL_SRCS} ${CACHEPOOL_SRCS} ${IMPDUBASE_SRCS} ${USERMODEL_SRCS} ${HOME_DIR}/Tests/test_usermodel.cpp)
target_link_libraries(test_usermodel pthread protobuf-lite slog mysqlclient hiredis)