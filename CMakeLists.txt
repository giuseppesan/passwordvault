cmake_minimum_required(VERSION 3.3.2)
project(PasswordVault)

find_package(OpenSSL REQUIRED)
set(CMAKE_CXX_STANDARD 11)

include(cmake/App.cmake)

set(SOURCES "core/ui_app/MyApp.h"
            "core/ui_app/MyApp.cpp"
            "core/main.cpp")

add_app("${SOURCES}")