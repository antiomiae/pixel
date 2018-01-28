set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pixel pixel/app/app.cpp pixel/app/app.h pixel/graphics/common.h pixel/graphics/texture.cpp pixel/graphics/texture.h pixel/graphics/graphics.h pixel/graphics/image.h pixel/graphics/image.cpp pixel/graphics/shader.cpp pixel/graphics/shader.h pixel/graphics/buffer.cpp pixel/graphics/buffer.h pixel/util/symbol_map.h pixel/util/symbol_map.cpp pixel/graphics/vao.cpp pixel/graphics/vao.h pixel/util/util.cpp pixel/util/util.h pixel/pixel.h pixel/math.h pixel/util/collections.h pixel/graphics/sprite.cpp pixel/graphics/sprite.h pixel/time/frame_counter.cpp pixel/time/frame_counter.h pixel/time/frame_rate_limiter.cpp pixel/time/frame_rate_limiter.h)
target_include_directories(pixel PUBLIC .)

add_subdirectory(demos)
add_subdirectory(tools)
add_subdirectory(test)

set(GLM_STATIC_LIBRARY_ENABLE OFF CACHE BOOL "" FORCE)
set(GLM_DYNAMIC_LIBRARY_ENABLE OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

target_link_libraries(pixel PUBLIC glfw)

target_link_libraries(pixel PUBLIC glew::glew)

add_subdirectory(lib/tinypng)
target_link_libraries(pixel PUBLIC tinypng)

target_include_directories(pixel PUBLIC lib/glm-0.9.9-a2)

if(CMAKE_C_FLAGS_DEBUG)
    target_compile_definitions(pixel PUBLIC PIXEL_DEBUG=1)
else(CMAKE_C_FLAGS_DEBUG)
    target_compile_definitions(pixel PUBLIC PIXEL_DEBUG=0)
endif(CMAKE_C_FLAGS_DEBUG)
