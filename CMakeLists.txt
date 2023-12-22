cmake_minimum_required(VERSION 3.10)
project(PasswordVault)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(clinterface_app)
add_subdirectory(crypto_app)

add_executable(main main.cpp main.hpp)

#link the apps 
target_link_libraries(main PRIVATE clinterface_app crypto_app)
