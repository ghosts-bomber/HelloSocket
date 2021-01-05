cmake_minimum_required(VERSION 3.15)
#羡慕名称
project(HelloSocket)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${PROJECT_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Release)

set(CMAKE_BUILD_TYPE "Debug")

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")
else( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -pthread -fopenmp")
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )

message("Build Type:"${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})

#add_definitions(-std=c++11)

add_subdirectory(EasyTCPClient)
add_subdirectory(EasyTCPSever)