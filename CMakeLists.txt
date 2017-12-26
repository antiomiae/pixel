cmake_minimum_required(VERSION 3.8)
project(main)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp)

add_subdirectory(pixel)
target_link_libraries(main pixel)
include_directories(main pixel)
