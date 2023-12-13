project(mergesort)
cmake_minimum_required(VERSION 3.0)

add_executable(mergesort main.cpp)
set_property(TARGET mergesort PROPERTY CXX_STANDARD 11)
target_link_libraries(mergesort pthread)
