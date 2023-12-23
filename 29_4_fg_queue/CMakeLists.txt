cmake_minimum_required(VERSION 3.5)

project(29_4_fg_queue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(29_4_fg_queue
  main.cpp
  node.h
  node.cpp
  list.h
  list.cpp
)

install(TARGETS 29_4_fg_queue
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
