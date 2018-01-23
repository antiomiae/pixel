cmake_minimum_required(VERSION 3.8)

include(pixel/cmake/HunterGate.cmake)

HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.19.232.tar.gz"
        SHA1 "a412c45fe4c5a72fed386f62dd8d753bd4fd3d11"
)

project(pixel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include(ExternalProject)

#include(pixel/cmake/GoogleTest.cmake)
#include(pixel/cmake/GLEW.cmake)
#include(pixel/cmake/LibPNG.cmake)

#add_custom_target(setup_deps)
#
#add_dependencies(setup_deps glew-external png-external)
#


# ExternalProject_Add(pixel
#        PREFIX ""
#        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pixel
#        BINARY_DIR ${PROJECT_BINARY_DIR}/pixel
#        DEPENDS png-external glew-external googletest
#        )

#add_subdirectory(pixel)

#add_dependencies(setup_deps png-external pixel)
