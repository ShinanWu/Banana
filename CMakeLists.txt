cmake_minimum_required(VERSION 3.10)
project(SeviceFramework)

set(CMAKE_CXX_STANDARD 11)

add_executable(SeviceFramework main.cpp ServiceManager.cpp ServiceManager.h MessageCenter.cpp MessageCenter.h Service.cpp Service.h ThreadPool.cpp ThreadPool.h Task.h ConcurrentRingBuffer.h WaitFreeConcurrentRingBuffer.h)