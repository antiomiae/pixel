cmake_minimum_required(VERSION 3.8)
project(gl)

set(CMAKE_CXX_STANDARD 17)

add_executable(gl main.cpp src/app/App.cpp src/app/App.h)
