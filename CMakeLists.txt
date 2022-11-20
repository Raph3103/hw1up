cmake_minimum_required(VERSION 3.17)
project(hw1up)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hw1up
        Commands.cpp
        Commands.h
        signals.cpp
        signals.h
        smash.cpp)
