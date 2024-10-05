cmake_minimum_required(VERSION 3.10)
project(CLInterfaceApp)

add_library(clinterface_app 
    src/CLInterface.cpp
)

target_include_directories(clinterface_app PUBLIC
    ${CMAKE_SOURCE_DIR}/core/clinterface_app/include
    ${CMAKE_SOURCE_DIR}/core/crypto_app/include
)

target_compile_definitions(clinterface_app PUBLIC PROJECT_ROOT="${PROJECT_ROOT}")

