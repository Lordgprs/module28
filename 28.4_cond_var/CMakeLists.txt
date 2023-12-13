project(polling)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)

add_executable(cond_var main.cpp)
set_property(TARGET cond_var PROPERTY CXX_STANDARD 11)
