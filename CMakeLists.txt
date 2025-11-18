cmake_minimum_required(VERSION 4.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp
        server.cpp
        server.h
        parser.cpp
        parser.h
        consoleManager.cpp
        consoleManager.h
        socketraii.cpp
        wsaRAII.cpp)
target_link_libraries(untitled ws2_32)

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()