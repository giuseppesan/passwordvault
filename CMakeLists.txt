cmake_minimum_required(VERSION 3.10)
project(PasswordVault)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(OpenSSL REQUIRED)

if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(core/clinterface_app)
add_subdirectory(core/crypto_app)
add_subdirectory(testing)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_subdirectory(ultralight_sdk)
endif()

add_executable(main core/main.cpp)
target_link_libraries(main PRIVATE crypto_app clinterface_app OpenSSL::SSL OpenSSL::Crypto)

# Google Tests
add_executable(MyGtests testing/main_test.cpp)

if(UNIX)
    if(APPLE)
        # macOS
        message("macOS detected")
        # Add macOS-specific configurations or settings here
        target_link_libraries(MyGtests PRIVATE mygtest GTest::GTest GTest::Main pthread OpenSSL::SSL OpenSSL::Crypto)
    else()
        # Linux
        message("Linux detected")
        # Add Linux-specific configurations or settings here
        target_link_libraries(MyGtests PRIVATE mygtest gtests gtest_main OpenSSL::SSL OpenSSL::Crypto)
    endif()
endif()
