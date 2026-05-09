cmake_minimum_required(VERSION 4.0)
project(AVLtree)

set(CMAKE_CXX_STANDARD 20)

add_executable(AVLtree main.cpp
        AVL.h
        AVL.cpp
        AVL_modified.h)
