# 指定CMake版本
cmake_minimum_required(VERSION 3.5)
 
# 指定项目名称
project(msg_server)
 
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
set(BASE_DIR ${HOME_DIR}/base)
set(3RD_DIR ${HOME_DIR}/3rdParty)
set(MSG_SERVER_DIR ${HOME_DIR}/msg_server)
set(TEST_DIR ${HOME_DIR}/tests_msgserver)

# message("HOME_DIR:${HOME_DIR}")

# JSON 源文件
aux_source_directory(${3RD_DIR}/jsoncpp SRC_JSON)

# IM自定义通信protobuf源文件
aux_source_directory(${3RD_DIR}/pb/protocol SRC_PB)

# protobuf-lite 库目录
SET(PB_LIB_DIR ${3RD_DIR}/pb/lib/linux)

# Mysql 头文件
SET(MYSQL_INCLUDE_DIR /usr/include/mysql)

# Mysql 库文件
SET(MYSQL_LIB /usr/lib64/mysql)


# Base 模块所有头文件
file(GLOB_RECURSE BASE_INC ${BASE_DIR}/*.h)
message("BASE_INC:${BASE_INC}")

# Base 模块所有源文件
file(GLOB_RECURSE BASE_SRCS ${BASE_DIR}/*.cpp)
file(GLOB_RECURSE SLOG_CMAKE_SRCS ${BASE_DIR}/SLog/*.cpp)
list(REMOVE_ITEM BASE_SRCS ${SLOG_CMAKE_SRCS})

# MSG_SERVER 模块所有头文件
file(GLOB_RECURSE MSG_SERVER_INC ${MSG_SERVER_DIR}/*.h)

# MSG_SERVER 模块所有源文件
file(GLOB_RECURSE MSG_SERVER_SRCS ${MSG_SERVER_DIR}/*.cpp)


# 指定头文件目录
include_directories(${3RD_DIR}/jsoncpp)
include_directories(${3RD_DIR} ${3RD_DIR}/pb ${3RD_DIR}/pb/protocol)
include_directories(${MYSQL_INCLUDE_DIR})
include_directories(${BASE_DIR} ${BASE_DIR}/Condition ${BASE_DIR}/Config ${BASE_DIR}/EncDec ${BASE_DIR}/HttpClient ${BASE_DIR}/HttpParser)
include_directories(${BASE_DIR}/ImConn ${BASE_DIR}/ImPduBase ${BASE_DIR}/Lock ${BASE_DIR}/NetLib ${BASE_DIR}/SLog)
include_directories(${BASE_DIR}/ThreadPool ${BASE_DIR}/Util ${BASE_DIR}/UtilPdu ${BASE_DIR}/ServInfo)
include_directories(${MSG_SERVER_DIR})


# attach data
link_directories(${BASE_DIR}/SLog ${PB_LIB_DIR})
link_libraries(slog)
add_executable(test_attachdata ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${MSG_SERVER_SRCS} ${TEST_DIR}/test_attachdata.cpp)
target_link_libraries(test_attachdata crypto pthread slog protobuf-lite mysqlclient curl hiredis)

# jsoncxx
add_executable(test_jsonxx ${MSG_SERVER_DIR}/jsonxx.cpp ${TEST_DIR}/test_jsonxx.cpp)

# connect file_server
link_directories(${BASE_DIR}/SLog ${PB_LIB_DIR})
link_libraries(slog)
add_executable(test_conn_filesvr ${SRC_JSON} ${SRC_PB} ${BASE_SRCS} ${MSG_SERVER_SRCS} ${TEST_DIR}/test_conn_filesvr.cpp)
target_link_libraries(test_conn_filesvr crypto pthread slog protobuf-lite mysqlclient curl hiredis)