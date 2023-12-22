cmake_minimum_required(VERSION 3.10)
project(CLInterfaceApp)

add_library(clinterface_app 
    src/CLInterface.cpp
)

target_include_directories(clinterface_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
