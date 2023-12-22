cmake_minimum_required(VERSION 3.10)

find_package(OpenSSL REQUIRED)

project(CryptoApp)

add_library(crypto_app 
    src/crypto.cpp 
    src/encryption.cpp 
    src/testing.cpp 
    src/utils.hpp
)

# Füge die Quelldateien und Header-Dateien hinzu
target_include_directories(crypto_app
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Verlinke mit OpenSSL-Bibliotheken
target_link_libraries(crypto_app PRIVATE OpenSSL::SSL OpenSSL::Crypto)
