// to configure the C++ build system

cmake_minimum_required(VERSION 3.15)
project(RTOS_Scheduler_Simulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(RTOS_Engine src/main.cpp)