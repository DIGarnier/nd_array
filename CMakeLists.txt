cmake_minimum_required(VERSION 3.11)
project(nd_array)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Werror")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -Werror")

add_executable(nd_array main.cpp nd_array.h)


