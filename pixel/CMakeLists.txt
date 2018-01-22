cmake_minimum_required(VERSION 3.8)
project(pixel CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(pixel SHARED pixel/app/app.cpp pixel/app/app.h pixel/graphics/common.h pixel/graphics/texture.cpp pixel/graphics/texture.h pixel/graphics/graphics.h pixel/graphics/image.h pixel/graphics/image.cpp pixel/graphics/shader.cpp pixel/graphics/shader.h pixel/graphics/buffer.cpp pixel/graphics/buffer.h pixel/util/symbol_map.h pixel/util/symbol_map.cpp pixel/graphics/vao.cpp pixel/graphics/vao.h pixel/util/util.cpp pixel/util/util.h pixel/pixel.h pixel/math.h pixel/util/collections.h pixel/tools/shadercheck.cpp pixel/tools/shadercheck.h pixel/graphics/sprite.cpp pixel/graphics/sprite.h pixel/time/frame_counter.cpp pixel/time/frame_counter.h pixel/time/frame_rate_limiter.cpp pixel/time/frame_rate_limiter.h)
target_include_directories(pixel PUBLIC .)

#target_link_libraries(pixel PUBLIC gtest)
add_subdirectory(${PROJECT_BINARY_DIR}/googletest-src)

add_subdirectory(demos)
add_subdirectory(pixel/tools)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(PNG_STATIC ON CACHE BOOL "" FORCE)
set(PNG_SHARED OFF CACHE BOOL "" FORCE)
set(PNG_FRAMEWORK OFF CACHE BOOL "" FORCE)

set(GLM_STATIC_LIBRARY_ENABLE OFF CACHE BOOL "" FORCE)
set(GLM_DYNAMIC_LIBRARY_ENABLE OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

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

target_include_directories(pixel PUBLIC lib/glm-0.9.9-a2)

if(CMAKE_C_FLAGS_DEBUG)
    target_compile_definitions(pixel PUBLIC PIXEL_DEBUG=1)
else(CMAKE_C_FLAGS_DEBUG)
    target_compile_definitions(pixel PUBLIC PIXEL_DEBUG=0)
endif(CMAKE_C_FLAGS_DEBUG)
