cmake_minimum_required(VERSION 3.10)
project(UIApp)

add_library(ui_app 
    src/UIApp.cpp
    src/MainFrame.cpp
    src/LoginDialog.cpp 
    src/CredentialsDialog.cpp
    src/RegisterDialog.cpp
)

target_include_directories(ui_app PUBLIC ${CMAKE_SOURCE_DIR}/UI/include
    ${CMAKE_SOURCE_DIR}/core/crypto_app/include
    ${CMAKE_SOURCE_DIR}/core/clinterface_app/include
)

target_compile_definitions(ui_app PUBLIC PROJECT_ROOT="${PROJECT_ROOT}")

