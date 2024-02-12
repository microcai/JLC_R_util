cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(minitoolsets CXX)


find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets )

qt_standard_project_setup()

include_directories(include)
include_directories(src)
include_directories(libs/qtcoro)

qt_add_executable(edatool 
    src/R_select_window.hpp
    src/mainwindow.hpp
    src/mainwindow.ui
    src/R_select_window.ui
    src/R_select_window.cpp
    src/mainwindow.cpp
    src/main.cpp
    src/R_gen.cpp
)

target_link_libraries(edatool PRIVATE Qt6::Widgets)

set_target_properties(edatool PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

qt_add_resources(edatool imageresources
    PREFIX "/images"
    FILES res/dcdc.png
)