cmake_minimum_required(VERSION 3.15)
project(csv)

set(CMAKE_CXX_STANDARD 14)

add_executable(csv main.cpp)

install(TARGETS csv DESTINATION bin)