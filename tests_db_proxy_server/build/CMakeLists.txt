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

# message("BASE_DIR:${BASE_DIR}")

# Base 模块所有头文件
file(GLOB_RECURSE BASE_INC ${BASE_DIR}/*.h)

# Base 模块所有源文件
file(GLOB_RECURSE BASE_SRCS ${BASE_DIR}/*.cpp)
file(GLOB_RECURSE SLOG_CMAKE_SRCS ${BASE_DIR}/SLog/*.cpp)
list(REMOVE_ITEM BASE_SRCS ${SLOG_CMAKE_SRCS})

# DB_PROXY_SERVER 模块所有头文件
file(GLOB_RECURSE DB_PROXY_SERVER_INC ${DB_PROXY_SERVER_DIR}/*.h)

# DB_PROXY_SERVER 模块所有源文件
file(GLOB_RECURSE DB_PROXY_SERVER_SRCS ${DB_PROXY_SERVER_DIR}/*.cpp)
file(GLOB_RECURSE DB_PROXY_SERVER_CMAKE_SRCS ${DB_PROXY_SERVER_DIR}/build/*.cpp)
list(REMOVE_ITEM DB_PROXY_SERVER_SRCS ${DB_PROXY_SERVER_CMAKE_SRCS})


# JSON 源文件
aux_source_directory(${HOME_DIR}/3rdParty/jsoncpp SRC_JSON)

# IM自定义通信protobuf源文件
aux_source_directory(${HOME_DIR}/3rdParty/pb/protocol SRC_PB)

# protobuf-lite 库目录
SET(PB_LIB_DIR ${HOME_DIR}/3rdParty/pb/lib/linux)

# Mysql 头文件
SET(MYSQL_INCLUDE_DIR /usr/include/mysql)

# Mysql 库文件
SET(MYSQL_LIB /usr/lib64/mysql)
 
# 指定头文件目录
include_directories(${BASE_DIR})
include_directories(${BASE_DIR}/SLog)
include_directories(${BASE_DIR}/Config)
include_directories(${BASE_DIR}/UtilPdu)
include_directories(${BASE_DIR}/EncDec)
include_directories(${BASE_DIR}/Condition)
include_directories(${BASE_DIR}/Lock)
include_directories(${BASE_DIR}/Util)
include_directories(${BASE_DIR}/ThreadPool)
include_directories(${BASE_DIR}/ImPduBase)
include_directories(${BASE_DIR}/NetLib)
include_directories(${BASE_DIR}/HttpParser)
include_directories(${BASE_DIR}/ImConn)
include_directories(${BASE_DIR}/HttpClient)
include_directories(${DB_PROXY_SERVER_DIR})
include_directories(${DB_PROXY_SERVER_DIR}/CachePool)
include_directories(${DB_PROXY_SERVER_DIR}/DbPool)
include_directories(${DB_PROXY_SERVER_DIR}/AudioModel)
include_directories(${DB_PROXY_SERVER_DIR}/DepartModel)
include_directories(${DB_PROXY_SERVER_DIR}/UserModel)
include_directories(${DB_PROXY_SERVER_DIR}/GroupModel)
include_directories(${DB_PROXY_SERVER_DIR}/FileModel)
include_directories(${DB_PROXY_SERVER_DIR}/SessionModel)
include_directories(${DB_PROXY_SERVER_DIR}/RelationModel)
include_directories(${DB_PROXY_SERVER_DIR}/GroupMessageModel)
include_directories(${DB_PROXY_SERVER_DIR}/MessageModel)
include_directories(${DB_PROXY_SERVER_DIR}/Login)
include_directories(${DB_PROXY_SERVER_DIR}/SyncCenter)
include_directories(${DB_PROXY_SERVER_DIR}/HandlerMap)
include_directories(${DB_PROXY_SERVER_DIR}/ProxyTask)
include_directories(${DB_PROXY_SERVER_DIR}/ProxyConn)

file(GLOB_RECURSE LOCK_SRCS ${BASE_DIR}/Lock/*.cpp)
file(GLOB_RECURSE UTIL_SRCS ${BASE_DIR}/Util/*.cpp)
file(GLOB_RECURSE CONFIG_SRCS ${BASE_DIR}/Config/*.cpp)
file(GLOB_RECURSE UTILPDU_SRCS ${BASE_DIR}/UtilPdu/*.cpp)
file(GLOB_RECURSE ENCDEC_SRCS ${BASE_DIR}/EncDec/*.cpp)
file(GLOB_RECURSE THREADPOOL_SRCS ${BASE_DIR}/ThreadPool/*.cpp)
file(GLOB_RECURSE IMPDUBASE_SRCS ${BASE_DIR}/ImPduBase/*.cpp)
file(GLOB_RECURSE NETLIB_SRCS ${BASE_DIR}/NetLib/*.cpp)
file(GLOB_RECURSE HTTPPARSER_SRCS ${BASE_DIR}/HttpParser/*.cpp)
file(GLOB_RECURSE IMCONN_SRCS ${BASE_DIR}/ImConn/*.cpp)
file(GLOB_RECURSE HTTPCLIENT_SRCS ${BASE_DIR}/HttpClient/*.cpp)

include_directories(${HOME_DIR}/3rdParty ${HOME_DIR}/3rdParty/pb ${HOME_DIR}/3rdParty/pb/protocol)
include_directories(${HOME_DIR}/3rdParty/jsoncpp)
include_directories(${MYSQL_INCLUDE_DIR})

link_directories(${BASE_DIR}/SLog)
link_libraries(slog)
link_directories(${PB_LIB_DIR})

# CachePool 依赖了 Config Util ThreadPool 使用了redis接口，依赖hiredis
file(GLOB_RECURSE CACHEPOOL_SRCS ${DB_PROXY_SERVER_DIR}/CachePool/*.cpp)
add_executable(test_cachepool  ${CONFIG_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${THREADPOOL_SRCS} ${CACHEPOOL_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_cachepool.cpp)
target_link_libraries(test_cachepool pthread hiredis slog)

# DbPool 依赖了 Config Lock Util ThreadPool 使用了mysql接口，依赖mysqlclient
file(GLOB_RECURSE DBPOOL_SRCS ${DB_PROXY_SERVER_DIR}/DbPool/*.cpp)
link_directories(${MYSQL_LIB})
add_executable(test_dbpool  ${CONFIG_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${THREADPOOL_SRCS} ${DBPOOL_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_dbpool.cpp)
target_link_libraries(test_dbpool pthread mysqlclient slog)

# AudioModel 依赖了 UtilPdu DbPool HttpClient 依赖第三方开源库 protobuf
file(GLOB_RECURSE AUDIOMODEL_SRCS ${DB_PROXY_SERVER_DIR}/AudioModel/*.cpp)
add_executable(test_audiomodel ${UTILPDU_SRCS} ${CONFIG_SRCS} ${LOCK_SRCS} ${UTIL_SRCS} ${THREADPOOL_SRCS} ${DBPOOL_SRCS} ${SRC_JSON} ${HTTPCLIENT_SRCS} ${SRC_PB} ${AUDIOMODEL_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_audiomodel.cpp)
target_link_libraries(test_audiomodel pthread mysqlclient curl protobuf-lite slog)

# DepartModel 依赖太麻烦了，直接包含所有吧
add_executable(test_departmodel  ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_departmodel.cpp)
target_link_libraries(test_departmodel crypto pthread slog protobuf-lite mysqlclient curl hiredis)


# UserModel 依赖太麻烦了，直接包含所有吧
add_executable(test_usermodel  ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_usermodel.cpp)
target_link_libraries(test_usermodel crypto pthread slog protobuf-lite mysqlclient curl hiredis)


# GroupModel 依赖太麻烦了，直接包含所有吧
add_executable(test_groupmodel  ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_groupmodel.cpp)
target_link_libraries(test_groupmodel crypto pthread slog protobuf-lite mysqlclient curl hiredis)


# FileModel 依赖太麻烦了，直接包含所有吧
add_executable(test_filemodel  ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_filemodel.cpp)
target_link_libraries(test_filemodel crypto pthread slog protobuf-lite mysqlclient curl hiredis)


# SessionModel 依赖太麻烦了，直接包含所有吧
add_executable(test_sessionmodel  ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_sessionmodel.cpp)
target_link_libraries(test_sessionmodel crypto pthread slog protobuf-lite mysqlclient curl hiredis)


# RelationModel 依赖太麻烦了，直接包含所有吧
add_executable(test_relationmodel  ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_relationmodel.cpp)
target_link_libraries(test_relationmodel crypto pthread slog protobuf-lite mysqlclient curl hiredis)


# GroupMessageModel 依赖太麻烦了，直接包含所有吧
add_executable(test_groupmsgmodel  ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_groupmsgmodel.cpp)
target_link_libraries(test_groupmsgmodel crypto pthread slog protobuf-lite mysqlclient curl hiredis)


# MessageModel 依赖太麻烦了，直接包含所有吧
add_executable(test_messagemodel  ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_messagemodel.cpp)
target_link_libraries(test_messagemodel crypto pthread slog protobuf-lite mysqlclient curl hiredis)

# DB_PROXY_SERVER Login 依赖太麻烦了，直接包含所有吧
add_executable(test_login ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_login.cpp)
target_link_libraries(test_login crypto pthread slog protobuf-lite mysqlclient curl hiredis)


# SyncCenter
add_executable(test_synccenter ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_synccenter.cpp)
target_link_libraries(test_synccenter crypto pthread slog protobuf-lite mysqlclient curl hiredis)

# ProxyConn
add_executable(test_proxyconn ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${HOME_DIR}/tests_db_proxy_server/test_proxyconn.cpp)
target_link_libraries(test_proxyconn crypto pthread slog protobuf-lite mysqlclient curl hiredis)