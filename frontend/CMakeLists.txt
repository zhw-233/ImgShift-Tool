cmake_minimum_required(VERSION 3.16)

project(ImgShift-Tool-Frontend VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(ImgShift-Tool
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
)

target_link_libraries(ImgShift-Tool PRIVATE Qt6::Widgets)
