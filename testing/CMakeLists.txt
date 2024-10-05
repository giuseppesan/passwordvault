cmake_minimum_required(VERSION 3.10)
project(MyGtests)

add_library(mygtest 
    main_test.cpp
    src/encryption_test.cpp
    src/hash_test.cpp
    src/mocking.cpp
    src/utils_test.cpp
)

target_include_directories(mygtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/core/crypto_app/include
    ${CMAKE_SOURCE_DIR}/core/clinterface_app/include
    ${CMAKE_SOURCE_DIR}/core/
)

target_compile_definitions(mygtest PUBLIC PROJECT_ROOT="${PROJECT_ROOT}")

