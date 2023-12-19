cmake_minimum_required(VERSION 3.5)

project(module29 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(module29 main.cpp)

install(TARGETS module29
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
