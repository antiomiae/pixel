cmake_minimum_required(VERSION 3.8)

project(tinypng)

set(CMAKE_CXX_STANDARD 17)

add_library(tinypng tinypng/png.cc tinypng/pixel.cc)

target_link_libraries(tinypng png)

target_include_directories(tinypng INTERFACE .
                                    PRIVATE ../libpng-1.6.34)
