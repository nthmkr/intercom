cmake_minimum_required(VERSION 2.8)

project(intercom)

set (CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(intercom main.cpp)
