cmake_minimum_required(VERSION 3.15)
project(Flows)

set(CMAKE_CXX_STANDARD 14)

add_executable(Flows main.cpp test.cpp preflow.h malhotra.cpp malhotra.h)

add_subdirectory(googletest)
target_link_libraries(Flows gtest gtest_main)