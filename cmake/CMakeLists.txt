cmake_minimum_required(VERSION 3.4)
project(SeviceFramework)
add_definitions(-g -lpthread -std=c++11)

#set(CMAKE_CXX_STANDARD 11)
aux_source_directory(.. SRC)
include_directories(../../glog/src/glog)
link_directories(../../glog/.libs/)
link_libraries(libglog.a -ldl -lpthread)

add_executable(SeviceFramework ${SRC})