cmake_minimum_required(VERSION 3.5)

project(30_7_quicksort LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(30_7_quicksort main.cpp
  threadpool.h
  threadpool.cpp
  requesthandler.h
  requesthandler.cpp
  BlockedQueue.h
  PromisedFunction.h
)

install(TARGETS 30_7_quicksort
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
