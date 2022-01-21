# 指定CMake版本
cmake_minimum_required(VERSION 3.5)
 
# 指定项目名称
project(test)
 
# 判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCC)
    message("COMPILER IS GNUCC")
    add_definitions ( -std=c++11 )
endif(CMAKE_COMPILER_IS_GNUCC)
 
# C++ 调试编译时使用的标志
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -ggdb3")
# C++ 发行编译时使用的标志
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

message("BASE_DIR:${BASE_DIR}")
 
# 指定头文件目录
include_directories(${BASE_DIR} ${BASE_DIR}/slog)
include_directories(${BASE_DIR}/config_m)
include_directories(${BASE_DIR}/utilPdu)
include_directories(${BASE_DIR}/encDec)
include_directories(${BASE_DIR}/threadpool)

#imPduBase依赖protobuf
SET(PB_LIB_DIR ${BASE_DIR}/3rdParty/pb/lib/linux)
include_directories(${BASE_DIR}/3rdParty ${BASE_DIR}/3rdParty/pb ${BASE_DIR}/3rdParty/pb/protocol ${BASE_DIR}/imPduBase)

# 指定链接库路径和链接库等并生成可执行文件
# slog 这里直接使用了自己生成的libslog.so而libslog.so依赖了第三方开源库log4cxx
link_directories(${BASE_DIR}/slog)
link_libraries(slog)
add_executable(test_slog ${BASE_DIR}/tests/test_slog.cpp)

# config
file(GLOB_RECURSE CONFIG_SRCS ${BASE_DIR}/config_m/*.cpp)
add_executable(test_config ${CONFIG_SRCS} ${BASE_DIR}/tests/test_config.cpp)

# utilpdu
file(GLOB_RECURSE UTILPDU_SRCS ${BASE_DIR}/utilPdu/*.cpp)
add_executable(test_utilpdu ${UTILPDU_SRCS} ${BASE_DIR}/tests/test_utilpdu.cpp)

# encdec 依赖了utilpdu 依赖系统库openssl/aes openssl/md5
file(GLOB_RECURSE ENCDEC_SRCS ${BASE_DIR}/encDec/*.cpp)
add_executable(test_encdec ${UTILPDU_SRCS} ${ENCDEC_SRCS} ${BASE_DIR}/tests/test_encdec.cpp)
target_link_libraries(test_encdec crypto)

# threadpool 依赖系统库pthread
file(GLOB_RECURSE THREADPOOL_SRCS ${BASE_DIR}/threadpool/*.cpp)
add_executable(test_threadpool ${THREADPOOL_SRCS} ${BASE_DIR}/tests/test_threadpool.cpp)
target_link_libraries(test_threadpool pthread)


# impdubase 依赖了utilpdu 依赖第三方开源库protobuf
aux_source_directory(${BASE_DIR}/3rdParty/pb/protocol SRC_PB)
link_directories(${PB_LIB_DIR})
file(GLOB_RECURSE IMPDUBASE_SRCS ${BASE_DIR}/imPduBase/*.cpp)
add_executable(test_impdubase ${UTILPDU_SRCS} ${IMPDUBASE_SRCS} ${SRC_PB} ${BASE_DIR}/tests/test_impdubase.cpp)
target_link_libraries(test_impdubase protobuf-lite)