cmake_minimum_required(VERSION 3.8)

project(tinypng CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(tinypng tinypng/png.cc tinypng/pixel.cc)

target_link_libraries(tinypng PUBLIC png)
target_include_directories(tinypng INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(tinypng PUBLIC ${EXTERNAL_PNG_INCLUDE_DIR})

