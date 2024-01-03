cmake_minimum_required(VERSION 3.10)
project(PasswordVault)

find_package(OpenSSL REQUIRED)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(clinterface_app)
add_subdirectory(crypto_app)
add_subdirectory(Testing)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE crypto_app clinterface_app OpenSSL::SSL OpenSSL::Crypto)


# Test-Executable
add_executable(MyGtests Testing/main_test.cpp)
target_link_libraries(MyGtests PRIVATE mygtest gtest gtest_main OpenSSL::SSL OpenSSL::Crypto)