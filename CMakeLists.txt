cmake_minimum_required(VERSION 3.10)
project(PasswordVault)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic)

# Define the project root relative to the source directory
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

# Find and include OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Find and include Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Find and include wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Add subdirectories
add_subdirectory(core/clinterface_app)
add_subdirectory(core/crypto_app)
add_subdirectory(testing)
add_subdirectory(UI)

# Create the main executable
add_executable(main core/main.cpp)
target_link_libraries(main PUBLIC clinterface_app crypto_app OpenSSL::SSL OpenSSL::Crypto)

# Create the Ui executable
add_executable(UIApp UI/src/UIApp.cpp)
target_link_libraries(UIApp PRIVATE ui_app clinterface_app crypto_app OpenSSL::SSL OpenSSL::Crypto ${wxWidgets_LIBRARIES})

# Create the Google Tests executable
add_executable(MyGtests testing/main_test.cpp)
target_link_libraries(MyGtests PRIVATE mygtest clinterface_app crypto_app GTest::GTest GTest::Main pthread OpenSSL::SSL OpenSSL::Crypto)
