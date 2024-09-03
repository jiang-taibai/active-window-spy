cmake_minimum_required(VERSION 3.29)
project(active_window_spy)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(active_window_spy main.cpp)
