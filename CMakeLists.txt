cmake_minimum_required(VERSION 4.0)
project(labs_1_2_sem3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ЛР-1: тесты структур данных
add_executable(lab1_tests
        tests/lab1/test.cpp
        tests/lab1/testCoreCopy.cpp
        tests/lab1/testAVLTree.cpp
        tests/lab1/testTreeDictionary.cpp
        tests/lab1/testTreeSet.cpp
        tests/lab1/testHashTable.cpp
        tests/lab1/testDictionaries.cpp
        tests/lab1/testTypes.h
        core/DynamicArray.h
        core/LinkedList.h
        core/AVLTree.h
        core/HashTable.h
        atd/KeyAndValue.h
        atd/IDictionary.h
        atd/TreeDictionary.h
        atd/HashDictionary.h
        atd/TreeSet.h
        my_except.h
)
target_compile_options(lab1_tests PRIVATE -fsanitize=address,undefined -g -O1)
target_link_options(lab1_tests PRIVATE -fsanitize=address,undefined)
