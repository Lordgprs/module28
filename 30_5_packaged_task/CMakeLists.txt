cmake_minimum_required(VERSION 3.5)

project(30_5_packaged_task LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(30_5_packaged_task main.cpp
  threadpool.h
  threadpool.cpp
  requesthandler.h
  requesthandler.cpp
  BlockedQueue.h
)

target_link_libraries(30_5_packaged_task -lpthread)

install(TARGETS 30_5_packaged_task
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
