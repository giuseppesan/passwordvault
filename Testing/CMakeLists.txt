cmake_minimum_required(VERSION 3.10)
project(MyGtests)

#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_library(mygtest 
    main_test.cpp
)

target_include_directories(mygtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


