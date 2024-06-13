cmake_minimum_required(VERSION 3.10) # Minimum version of CMake required

# Project name and language
project(Tests LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable target with source files
add_executable(Tests ../src/test.cpp)