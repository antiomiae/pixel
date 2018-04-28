cmake_minimum_required(VERSION 3.8)
project(pixel_test CXX)

file(GLOB test_sources ${CMAKE_CURRENT_LIST_DIR}/**/test_*.cpp)

add_executable(pixel_test
        main.cpp
        setup.cpp ${test_sources}
        )

target_link_libraries(pixel_test
        PUBLIC pixel GTest::gtest)

#target_compile_options(pixel_test
#        PUBLIC -static-libstdc++)
