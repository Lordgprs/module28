project(polling)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)

add_executable(polling main.cpp)
set_property(TARGET polling PROPERTY CXX_STANDARD 11)
