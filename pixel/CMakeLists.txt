cmake_minimum_required(VERSION 3.8)
project(pixel CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pixel SHARED app/app.cpp app/app.h graphics/common.h graphics/texture.cpp graphics/texture.h graphics/graphics.h graphics/image.h graphics/image.cpp graphics/shader.cpp graphics/shader.h graphics/buffer.cpp graphics/buffer.h util/symbol_map.h util/symbol_map.cpp graphics/vao.cpp graphics/vao.h util/util.cpp util/util.h pixel.h math.h util/collections.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL FORCE "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL FORCE "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL FORCE "")

set(PNG_STATIC ON)
set(PNG_SHARED OFF)
set(PNG_FRAMEWORK OFF)

#set(BOX2D_BUILD_SHARED ON CACHE BOOL "" FORCE)
#set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)
target_link_libraries(pixel PUBLIC glfw)

add_subdirectory(lib/glew-2.1.0/build/cmake)
target_link_libraries(pixel PUBLIC glew)

add_subdirectory(lib/libpng-1.6.34)

add_subdirectory(lib/tinypng)
target_link_libraries(pixel PUBLIC tinypng)

#add_subdirectory(lib/Box2D)
#target_link_libraries(pixel box2d)

add_subdirectory(lib/glm-0.9.9-a2)
target_link_libraries(pixel PUBLIC glm)
target_include_directories(pixel INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
