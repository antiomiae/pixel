set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
        pixel/system/version.h pixel/system/version.cpp
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
        pixel/lua/lua.h pixel/tilemap/tileset.cpp pixel/tilemap/tileset.h pixel/tilemap/tile_layer_texture.cpp pixel/tilemap/tile_layer_texture.h pixel/lua/lua_binding.cpp pixel/graphics/pack.cpp pixel/graphics/pack.h pixel/graphics/texture_atlas.cpp pixel/graphics/texture_atlas.h)

target_include_directories(pixel PUBLIC .)

add_subdirectory(demos)
add_subdirectory(tools)
add_subdirectory(test)
add_subdirectory(lib/tinypng)

set(TMXLITE_STATIC_LIB ON)
add_subdirectory(lib/tmxlite/tmxlite)

add_subdirectory(lib/sol2)

target_link_libraries(pixel
        PUBLIC Boost::system Boost::filesystem tinypng glfw glew::glew glm tmxlite
        Lua::lua_lib
        sol2
        )

#target_include_directories(pixel PUBLIC lib/glm-0.9.9-a2)

target_compile_definitions(pixel
        PUBLIC
        PIXEL_VERSION_STRING="0.0.1"
        )

if (CMAKE_C_FLAGS_DEBUG)
    target_compile_definitions(pixel PUBLIC PIXEL_DEBUG=1)
else (CMAKE_C_FLAGS_DEBUG)
    target_compile_definitions(pixel PUBLIC PIXEL_DEBUG=0)
endif (CMAKE_C_FLAGS_DEBUG)
target_compile_definitions(pixel PUBLIC _DEFAULT_SOURCE=1)
