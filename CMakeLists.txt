cmake_minimum_required(VERSION 3.10)
project(PasswordVault)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wextra -Wpedantic)

# Find and include OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Find and include Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(core/clinterface_app)
add_subdirectory(core/crypto_app)
add_subdirectory(testing)

# Conditionally add ultralight_sdk based on the system
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_subdirectory(ultralight_sdk)
else()
    # add_subdirectory(ultralight_sdk_mac)
endif()

# Create the main executable
add_executable(main core/main.cpp)
target_link_libraries(main PRIVATE crypto_app clinterface_app OpenSSL::SSL OpenSSL::Crypto)

# Create the Google Tests executable
add_executable(MyGtests testing/main_test.cpp)
target_link_libraries(MyGtests PRIVATE mygtest GTest::GTest GTest::Main pthread OpenSSL::SSL OpenSSL::Crypto)
