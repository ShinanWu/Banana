cmake_minimum_required(VERSION 3.4)
project(SeviceFramework)
add_definitions(-g -lpthread -std=c++11)

#set(CMAKE_CXX_STANDARD 11)
aux_source_directory(.. SRC)
include_directories(..)
include_directories(../../glog/src/glog)
link_directories(../../glog/.libs/)
link_libraries(libglog.a -ldl -lpthread)

add_executable(SeviceFramework ${SRC} ../Task.h ../InteractiveTask.cpp ../IndependentTask.cpp ../ConcurrentHashMap.h ../examples/NetService.cpp ../examples/NetService.h ../network/LibeventReactor.cpp ../network/LibeventReactor.h ../examples/StringStreamParser.cpp ../examples/StringStreamParser.h ../network/EventReactor.cpp ../network/EventReactor.h ../network/Connection.cpp ../network/Connection.h ../network/StreamParser.h ../network/SelectEventReactor.cpp ../network/SelectEventReactor.h)