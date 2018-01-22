cmake_minimum_required(VERSION 3.8)
project(tools)

add_executable(shadercheck shadercheck.cpp)
target_link_libraries(shadercheck pixel)
