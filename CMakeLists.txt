cmake_minimum_required(VERSION 2.8.11)

#Creating a new project
project(funcOpt)
#Executables are built in the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#Looking for Qt5 library
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

#Adding the truncated_normal library
add_subdirectory(lib/truncated_normal)
include_directories(lib/truncated_normal)

#Building the project in src
add_subdirectory(src)
