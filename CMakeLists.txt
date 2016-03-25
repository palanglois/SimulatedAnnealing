cmake_minimum_required(VERSION 2.8.11)

project(funcOpt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

add_executable(
	funcOpt
	main.cpp
	funcOpt.h
	truncated_normal.cpp
	truncated_normal.hpp
	Prony.cpp
	Prony.h
	Graph.h
	AlgoThread.h
	DispThread.h
)

target_link_libraries(funcOpt Qt5::Widgets)

set(CMAKE_CXX_FLAGS "-std=c++11")