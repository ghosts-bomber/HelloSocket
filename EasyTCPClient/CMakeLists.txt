set(ProjectName "EasyTCPClient")
project(${ProjectName})
#添加源文件
#aux_source_directory(./src/network NETWORK_SRCS)
file(GLOB NETWORK_SRCS ./src/network/*.* )
file(GLOB DIR_SRCS ./src/*.c* ./src/*.h* ./src/*.h ./src/*.c)
#include_directories(./include)
# 代码归组
#source_group(network FILES ${NETWORK_SRCS})
# 生成可执行文件
add_executable(${ProjectName} ${DIR_SRCS} ${NETWORK_SRCS})