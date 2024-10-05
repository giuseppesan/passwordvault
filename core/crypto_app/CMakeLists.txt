cmake_minimum_required(VERSION 3.10)
project(CryptoApp)

add_library(crypto_app 
    src/hash.cpp 
    src/encryption.cpp 
)

target_include_directories(crypto_app PUBLIC ${CMAKE_SOURCE_DIR}/core/crypto_app/include)
target_compile_definitions(crypto_app PUBLIC PROJECT_ROOT="${PROJECT_ROOT}")
