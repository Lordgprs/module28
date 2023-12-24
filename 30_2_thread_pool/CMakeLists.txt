cmake_minimum_required(VERSION 3.5)

project(30_2_thread_pool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(30_2_thread_pool main.cpp
  threadpool.h
  threadpool.cpp
  requesthandler.h
  requesthandler.cpp
)

install(TARGETS 30_2_thread_pool
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
