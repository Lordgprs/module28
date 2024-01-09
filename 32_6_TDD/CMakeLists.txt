cmake_minimum_required(VERSION 3.5)

project(32_6_TDD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(32_6_TDD main.cpp)

install(TARGETS 32_6_TDD
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
target_link_libraries(32_6_TDD gtest gmock pthread)
