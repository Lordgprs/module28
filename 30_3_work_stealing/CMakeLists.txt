cmake_minimum_required(VERSION 3.5)

project(30_3_work_stealing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(30_3_work_stealing main.cpp
  threadpool.h
  threadpool.cpp
  requesthandler.h
  requesthandler.cpp
  BlockedQueue.h
)

install(TARGETS 30_3_work_stealing
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
