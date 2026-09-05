cmake_minimum_required(VERSION 3.10)
project(UberRidePlatform CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

# Main.cpp pulls in the class files for this teaching/demo build.
add_executable(UberRidePlatform Main.cpp)
