project(packaged_task)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)

add_executable(packaged_task main.cpp)
set_property(TARGET packaged_task PROPERTY CXX_STANDARD 11)
