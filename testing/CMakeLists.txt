cmake_minimum_required(VERSION 3.10)
project(MyGtests)

add_library(mygtest 
    main_test.cpp
    src/encryption_test.cpp
    src/hash_test.cpp
    src/mocking.cpp
    src/utils_test.cpp
)

target_include_directories(mygtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


