cmake_minimum_required(VERSION 3.5)

project(30_5_2_promise LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(30_5_2_promise main.cpp
  threadpool.h
  threadpool.cpp
  requesthandler.h
  requesthandler.cpp
  BlockedQueue.h
  PromisedFunction.h
)

target_link_libraries(30_5_2_promise -lpthread)

install(TARGETS 30_5_2_promise
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
