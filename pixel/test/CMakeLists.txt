cmake_minimum_required(VERSION 3.8)
#project(pixel_test CXX)

add_executable(pixel_test test_sprite.cpp)

target_link_libraries(pixel_test PUBLIC pixel)
target_link_libraries(pixel_test PUBLIC GTest::gtest)
target_link_libraries(pixel_test PUBLIC GTest::main)
