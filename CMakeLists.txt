cmake_minimum_required(VERSION 3.23)
project(membox2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(membox2
        page.cpp
        page.h
        memory.h SV39.h)
