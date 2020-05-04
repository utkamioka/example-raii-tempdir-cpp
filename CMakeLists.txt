cmake_minimum_required(VERSION 3.16)
project(example_raii_tempdir)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_FLAGS_DEBUG -DDEBUG)

add_executable(example_raii_tempdir main.cpp src/filesystem_utils.cpp src/filesystem_utils.h)
