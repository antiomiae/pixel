cmake_minimum_required(VERSION 3.9)

cmake_policy(SET CMP0042 NEW)

include(cmake/HunterGate.cmake)

HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.20.73.tar.gz"
        SHA1 "5cd594b2bb12b2d66983a40b230f760187abbe5a"
        LOCAL
)

project(pixel_main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif (APPLE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (UNIX AND NOT APPLE)
    set(HUNTER_BUILD_SHARED_LIBS OFF)
    set(BUILD_SHARED_LIBS OFF)
else (UNIX AND NOT APPLE)
    set(HUNTER_BUILD_SHARED_LIBS ON)
    set(BUILD_SHARED_LIBS ON)
endif (UNIX AND NOT APPLE)

hunter_add_package(glew)
hunter_add_package(glfw)
hunter_add_package(Lua)
#hunter_add_package(Box2D)
hunter_add_package(Boost COMPONENTS system filesystem)

find_package(glew CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Lua CONFIG REQUIRED)
#find_package(Box2D CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED system filesystem)

add_subdirectory(pixel)

ADD_SUBDIRECTORY(pixel/demos)
ADD_SUBDIRECTORY(pixel/tools)
ADD_SUBDIRECTORY(pixel/test)
ADD_SUBDIRECTORY(pixel/pixel/entrypoints)
