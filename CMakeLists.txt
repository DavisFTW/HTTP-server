cmake_minimum_required(VERSION 4.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp
        server.cpp
        server.h)
target_link_libraries(untitled ws2_32)