cmake_minimum_required(VERSION 3.10)

# set the project name
project(CST8703Lab1
    VERSION 1.0
    LANGUAGES CXX C
    DESCRIPTION "Lab 1: First Real-Time Kernel Jitters")

# compiler flags and warnings
set(PROJECT_COMPILER_OPTIONS
    -pedantic
    -Werror
    -Wextra
    -Wall
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wrestrict
    -Wnull-dereference
    -Wjump-misses-init
    -Wdouble-promotion
    -Wshadow
    -Wformat=2)
set(PROJECT_C_COMPILER_OPTIONS
    ${PROJECT_COMPILER_OPTIONS})
set(PROJECT_CXX_COMPILER_OPTIONS
    ${PROJECT_COMPILER_OPTIONS}
    -Wold-style-cast
    -Wuseless-cast)

# required packages and libraries
find_package(Threads REQUIRED)
find_library(LIBRT rt REQUIRED)
find_library(LIBM m REQUIRED)

# set to variable
set(PROJECT_REQUIRED_LIBRARIES
    Threads::Threads rt m)

# add subfolders
add_subdirectory(JitteryTimers)
