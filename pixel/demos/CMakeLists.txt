cmake_minimum_required(VERSION 3.8)
project(demos)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp)

target_link_libraries(main pixel)
