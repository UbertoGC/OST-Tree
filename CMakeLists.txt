cmake_minimum_required(VERSION 3.24)
project(OST_Tree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(OST_Tree
        main.cpp
        OST_Tree.h
        OST_Tree_nodo.h)
