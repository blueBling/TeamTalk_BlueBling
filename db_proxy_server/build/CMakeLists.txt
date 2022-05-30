# 指定CMake版本
cmake_minimum_required(VERSION 3.5)
 
# 指定项目名称
project(db_proxy_server)
 
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

# ImPduBase 依赖 protobuf
include_directories(${HOME_DIR}/3rdParty ${HOME_DIR}/3rdParty/pb ${HOME_DIR}/3rdParty/pb/protocol)

# HttpClient 依赖 jsoncpp
include_directories(${HOME_DIR}/3rdParty/jsoncpp)

# DbPool 依赖 mysql
include_directories(${MYSQL_INCLUDE_DIR})


# db_proxy_server
link_directories(${BASE_DIR}/SLog ${PB_LIB_DIR})
link_libraries(slog)
add_executable(db_proxy_server ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${DB_PROXY_SERVER_SRCS} ${DB_PROXY_SERVER_DIR}/build/db_proxy_server.cpp)
target_link_libraries(db_proxy_server crypto pthread slog protobuf-lite mysqlclient curl hiredis)