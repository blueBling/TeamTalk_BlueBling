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

# 添加该工程编译时所需的依赖库
# 依赖库所在路径
link_directories(${BASE_DIR}/slog)
# 链接依赖库
link_libraries(slog)
 
# 添加该工程编译完成后的可执行文件
add_executable(test_slog ${BASE_DIR}/tests/test_slog.cpp)

file(GLOB_RECURSE CONFIG_SRCS ${BASE_DIR}/config_m/*.cpp)
add_executable(test_config ${CONFIG_SRCS} ${BASE_DIR}/tests/test_config.cpp)

file(GLOB_RECURSE UTILPDU_SRCS ${BASE_DIR}/utilPdu/*.cpp)
add_executable(test_utilpdu ${UTILPDU_SRCS} ${BASE_DIR}/tests/test_utilpdu.cpp)

file(GLOB_RECURSE ENCDEC_SRCS ${BASE_DIR}/encDec/*.cpp)
add_executable(test_encdec ${ENCDEC_SRCS} ${BASE_DIR}/tests/test_encdec.cpp)
target_link_libraries(test_encdec crypto)
