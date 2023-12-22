cmake_minimum_required(VERSION 3.10)
project(CLInterfaceApp)

add_library(clinterface_app 
    src/CLInterface.cpp)

# Füge die Quelldateien und Header-Dateien hinzu
target_include_directories(clinterface_app
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../crypto_app/include
)

# Verlinke mit crypto_app und OpenSSL-Bibliotheken
target_link_libraries(clinterface_app PRIVATE crypto_app)
