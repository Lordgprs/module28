project(mtqueue)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)

add_executable(mtqueue main.cpp)
set_property(TARGET mtqueue PROPERTY CXX_STANDARD 17)
