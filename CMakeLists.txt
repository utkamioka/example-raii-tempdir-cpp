cmake_minimum_required(VERSION 2.8)
project(example_raii_tempdir)

#set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_CXX_STANDARD 11)  # do not work(?) on cmake-2.8. use "target_compile_options(<target> -std=c++11)".
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")

file(GLOB SOURCES RELATIVE ${CMAKE_HOME_DIRECTORY} "src/*.cpp")
file(GLOB TEST_SOURCES RELATIVE ${CMAKE_HOME_DIRECTORY} "test/*.cpp")

add_executable(${PROJECT_NAME}-main main.cpp ${SOURCES})
target_compile_options(${PROJECT_NAME}-main PRIVATE -W -Wall)
target_compile_options(${PROJECT_NAME}-main PRIVATE -std=c++11)
target_include_directories(${PROJECT_NAME}-main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ------------------------------------------------------------------------

#
# https://github.com/google/googletest/blob/master/googletest/README.md
#

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_executable(${PROJECT_NAME}-googletest ${SOURCES} ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME}-googletest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}-googletest gtest_main)

enable_testing()  # for use 'make test'
add_test(COMMAND ${PROJECT_NAME}-googletest)
# ------------------------------------------------------------------------
