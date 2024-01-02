cmake_minimum_required(VERSION 3.10)
project(CryptoApp)

add_library(crypto_app 
    src/crypto.cpp 
    src/encryption.cpp 
    #src/testing.cpp 
    src/utils.cpp
)

target_include_directories(crypto_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
