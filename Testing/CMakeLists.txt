cmake_minimum_required(VERSION 3.10)
project(MyGtests)

add_library(mygtest 
    main_test.cpp
    testing.cpp
)

target_include_directories(mygtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


