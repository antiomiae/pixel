set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(${PROJECT_SOURCE_DIR}/cmake/PixelVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/generated/PixelVersion.cmake @ONLY)

add_custom_target(pixel_source_version ALL
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/generated/PixelVersion.cmake
        )

add_library(pixel
        pixel/app/app.cpp pixel/app/app.h
        pixel/graphics/common.h
        pixel/graphics/texture.cpp pixel/graphics/texture.h
        pixel/graphics/graphics.h
        pixel/graphics/image.h pixel/graphics/image.cpp
        pixel/graphics/shader.cpp pixel/graphics/shader.h
        pixel/graphics/buffer.cpp pixel/graphics/buffer.h
        pixel/util/symbol_map.h pixel/util/symbol_map.cpp
        pixel/graphics/vao.cpp pixel/graphics/vao.h
        pixel/util/util.cpp pixel/util/util.h
        pixel/pixel.h
        pixel/math/math.h
        pixel/util/collections.h
        pixel/graphics/sprite.cpp pixel/graphics/sprite.h
        pixel/time/frame_counter.cpp pixel/time/frame_counter.h
        pixel/time/frame_rate_limiter.cpp pixel/time/frame_rate_limiter.h
        pixel/system/version.h ${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp
        pixel/system/system.h
        pixel/graphics/sprite_renderer.cpp pixel/graphics/sprite_renderer.h
        pixel/tilemap/tile_layer.h pixel/tilemap/tile_atlas.cpp
        pixel/tilemap/tile_atlas.h pixel/tilemap/tile_layer.cpp
        pixel/tilemap/tile_map.cpp pixel/tilemap/tile_map.h
        pixel/tilemap/tile_map_renderer.cpp pixel/tilemap/tile_map_renderer.h
        pixel/graphics/shader_utils.cpp pixel/graphics/shader_utils.h
        pixel/graphics/attribute.cpp pixel/graphics/attribute.h
        pixel/graphics/render_context.cpp pixel/graphics/render_context.h
        pixel/error.h
        pixel/tilemap/tileset.cpp pixel/tilemap/tileset.h
        pixel/tilemap/tile_layer_texture.cpp pixel/tilemap/tile_layer_texture.h
        pixel/graphics/pack.cpp pixel/graphics/pack.h
        pixel/graphics/texture_atlas.cpp pixel/graphics/texture_atlas.h
        pixel/graphics/render_buffer.cpp pixel/graphics/render_buffer.h
        pixel/graphics/sprite_batch.cpp pixel/graphics/sprite_batch.h
        pixel/graphics/camera.cpp pixel/graphics/camera.h
        pixel/graphics/sprite_animation.cpp pixel/graphics/sprite_animation.h
        pixel/input/keyboard.cpp pixel/input/keyboard.h pixel/lua/wrap_glm.cpp pixel/lua/wrap_camera.cpp pixel/lua/wrap_image_data.cpp
        pixel/lua/binding.h pixel/lua/binding.cpp
        pixel/graphics/offscreen_render_target.cpp pixel/graphics/offscreen_render_target.h pixel/lua/wrap_opengl.cpp pixel/lua/wrap_tileset.cpp pixel/collision/collision.cpp pixel/collision/collision.h pixel/types.h pixel/lua/wrap_collision.cpp)


add_dependencies(pixel pixel_source_version)

target_include_directories(pixel PUBLIC .)

add_subdirectory(demos)
add_subdirectory(tools)
add_subdirectory(test)
add_subdirectory(lib/tinypng)
add_subdirectory(pixel/entrypoints)

set(TMXLITE_STATIC_LIB ON)
add_subdirectory(lib/tmxlite/tmxlite)

add_subdirectory(lib/sol2)

target_link_libraries(pixel
        PUBLIC Boost::system Boost::filesystem tinypng glfw glew::glew glm tmxlite
        Lua::lua_lib
        sol2
        )

if (CMAKE_C_FLAGS_DEBUG)
    target_compile_definitions(pixel PUBLIC PIXEL_DEBUG=1)
else (CMAKE_C_FLAGS_DEBUG)
    target_compile_definitions(pixel PUBLIC PIXEL_DEBUG=0)
endif (CMAKE_C_FLAGS_DEBUG)

target_compile_definitions(pixel PUBLIC _DEFAULT_SOURCE=1)

