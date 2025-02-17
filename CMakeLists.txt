cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# cmake_policy(SET CMP0091 NEW)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(minitoolsets CXX)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-foptimize-sibling-calls)
endif()

if (MSVC)
    add_link_options(/NODEFAULTLIB:libucrt$<$<CONFIG:Debug>:d>.lib)
    link_libraries(ucrt$<$<CONFIG:Debug>:d>.lib)
endif(MSVC)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets )

qt_standard_project_setup()

include_directories(include)
include_directories(src)
include_directories(libs/qtcoro)

qt_add_executable(edatool
    src/mainwindow.ui
    src/R_select_window.ui
    src/I_Calc.ui
    src/LLC_window.ui

    src/R_select_window.hpp
    src/mainwindow.hpp
    src/I_Calc.hpp
    src/LLC_window.hpp

    src/R_select_window.cpp
    src/mainwindow.cpp
    src/I_Calc.cpp
    src/LLC_window.cpp
    src/main.cpp
    src/R_gen.cpp
)

target_link_libraries(edatool PRIVATE Qt6::Widgets)

if (MSVC)
target_link_options(edatool PRIVATE "/NODEFAULTLIB")
target_link_libraries(edatool PRIVATE ucrt$<$<CONFIG:Debug>:d>.lib msvcrt.lib libvcruntime.lib libcpmt$<$<CONFIG:Debug>:d>.lib)
endif()

set_target_properties(edatool PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

qt_add_resources(edatool imageresources
    PREFIX "/images"
    FILES res/dcdc.png res/logo.png
)

install(TARGETS edatool DESTINATION bin)
