cmake_minimum_required(VERSION 3.10)
project(PasswordVault)

set(CMAKE_CXX_STANDARD 11)

# Füge die Unterverzeichnisse hinzu
add_subdirectory(clinterface_app)
add_subdirectory(crypto_app)

# Füge die ausführbare Datei für das Hauptprogramm hinzu
add_executable(main main.cpp main.hpp)

# Füge die ausführbaren Dateien aus den Unterverzeichnissen hinzu
target_link_libraries(main PRIVATE clinterface_app crypto_app)
