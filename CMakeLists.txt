cmake_minimum_required(VERSION 4.0)
project(labs_1_2_sem3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ЛР-1: тесты структур данных
add_executable(lab1_tests
        tests/lab1/test.cpp
        tests/lab1/testBinarySearchTree.cpp
        core/BinarySearchTree.h
        my_except.h
)
target_compile_options(lab1_tests PRIVATE -fsanitize=address,undefined -g -O1)
target_link_options(lab1_tests PRIVATE -fsanitize=address,undefined)
