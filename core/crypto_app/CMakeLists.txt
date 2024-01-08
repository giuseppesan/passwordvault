cmake_minimum_required(VERSION 3.10)
project(CryptoApp)

add_library(crypto_app 
    src/hash.cpp 
    src/encryption.cpp 
)

target_include_directories(crypto_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
