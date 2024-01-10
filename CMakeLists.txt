cmake_minimum_required(VERSION 3.10)
project(PasswordVault)

find_package(OpenSSL REQUIRED)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(core/clinterface_app)
add_subdirectory(core/crypto_app)
add_subdirectory(testing)
add_subdirectory(ultralight_sdk)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(main core/main.cpp)
target_link_libraries(main PRIVATE crypto_app clinterface_app OpenSSL::SSL OpenSSL::Crypto)

# Test-Executable
add_executable(MyGtests testing/main_test.cpp)
target_link_libraries(MyGtests PRIVATE mygtest gtest gtest_main OpenSSL::SSL OpenSSL::Crypto)