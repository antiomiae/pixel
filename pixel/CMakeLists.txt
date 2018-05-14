SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/PixelVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/generated/PixelVersion.cmake @ONLY)

ADD_CUSTOM_TARGET(pixel_source_version ALL
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/generated/PixelVersion.cmake
    )

ADD_LIBRARY(pixel
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
    pixel/pixel.h pixel/math.h
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
    pixel/graphics/sprite_batch.cpp pixel/graphics/sprite_batch.h
    pixel/graphics/camera.cpp pixel/graphics/camera.h
    pixel/graphics/sprite_animation.cpp pixel/graphics/sprite_animation.h
    pixel/input/keyboard.cpp pixel/input/keyboard.h pixel/lua/wrap_glm.cpp pixel/lua/wrap_camera.cpp pixel/lua/wrap_image_data.cpp
    pixel/lua/binding.h pixel/lua/binding.cpp
    pixel/graphics/offscreen_render_target.cpp pixel/graphics/offscreen_render_target.h
    pixel/lua/wrap_opengl.cpp
    pixel/lua/wrap_tileset.cpp
    pixel/collision/collision.cpp pixel/collision/collision.h
    pixel/types.h
    pixel/lua/wrap_collision.cpp pixel/renderers/line_renderer.cpp pixel/renderers/line_renderer.h
    )


ADD_DEPENDENCIES(pixel pixel_source_version)

TARGET_INCLUDE_DIRECTORIES(pixel PUBLIC .)

SET(TMXLITE_STATIC_LIB ON)

ADD_SUBDIRECTORY(demos)
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(pixel/entrypoints)
ADD_SUBDIRECTORY(lib/tinypng)
ADD_SUBDIRECTORY(lib/glm)
ADD_SUBDIRECTORY(lib/sol2)
ADD_SUBDIRECTORY(lib/tmxlite/tmxlite)


TARGET_LINK_LIBRARIES(pixel
    PUBLIC Boost::system Boost::filesystem tinypng glfw glew::glew tmxlite
    Lua::lua_lib
    sol2
    )

IF (CMAKE_C_FLAGS_DEBUG)
  TARGET_COMPILE_DEFINITIONS(pixel PUBLIC PIXEL_DEBUG=1)
ELSE (CMAKE_C_FLAGS_DEBUG)
  TARGET_COMPILE_DEFINITIONS(pixel PUBLIC PIXEL_DEBUG=0)
ENDIF (CMAKE_C_FLAGS_DEBUG)

TARGET_COMPILE_DEFINITIONS(pixel PUBLIC _DEFAULT_SOURCE=1)

