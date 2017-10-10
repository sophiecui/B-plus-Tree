cmake_minimum_required(VERSION 3.7)
project(BPlus)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp bplus.cpp.cpp bplus.h node.cpp)
add_executable(BPlus ${SOURCE_FILES})