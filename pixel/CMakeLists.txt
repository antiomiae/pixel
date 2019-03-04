project(pixel)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

CONFIGURE_FILE(${pixel_main_SOURCE_DIR}/cmake/PixelVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/generated/PixelVersion.cmake @ONLY)

ADD_CUSTOM_TARGET(pixel_source_version ALL
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/generated/PixelVersion.cmake
    )

OPTION(PIXEL_LUA_BINDING "build lua binding" OFF)

list(APPEND pixel_sources
    ${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp
    pixel/app/app.cpp
    pixel/app/app.h
    pixel/collision/collision.cpp
    pixel/collision/collision.h
    pixel/error.h
    pixel/graphics/buffer.cpp
    pixel/graphics/common.h
    pixel/graphics/graphics.cpp
    pixel/graphics/graphics.h
    pixel/graphics/image.cpp
    pixel/graphics/offscreen_render_target.cpp
    pixel/graphics/pack.cpp
    pixel/graphics/pack.h
    pixel/graphics/render_context.cpp
    pixel/graphics/render_context.h
    pixel/renderers/line_renderer.cpp
    pixel/renderers/line_renderer.h
    pixel/renderers/renderers.h
    pixel/renderers/sprite_renderer.cpp
    pixel/renderers/sprite_renderer.h
    pixel/renderers/tile_map_renderer.cpp
    pixel/renderers/tile_map_renderer.h
    pixel/graphics/shader.cpp
    pixel/graphics/shader_utils.cpp
    pixel/graphics/shader_utils.h
    pixel/graphics/sprite.cpp
    pixel/graphics/sprite.h
    pixel/graphics/texture_atlas.cpp
    pixel/input/input.cpp
    pixel/input/input.h
    pixel/math.h
    pixel/pixel.h
    pixel/system/system.cpp
    pixel/system/system.h
    pixel/system/version.h
    pixel/tilemap/tile_atlas.cpp
    pixel/tilemap/tile_atlas.h
    pixel/tilemap/tile_layer.cpp
    pixel/tilemap/tile_layer.h
    pixel/tilemap/tile_layer_texture.cpp
    pixel/tilemap/tile_layer_texture.h
    pixel/tilemap/tile_map.cpp
    pixel/tilemap/tile_map.h
    pixel/tilemap/tileset.cpp
    pixel/tilemap/tileset.h
    pixel/time/frame_counter.cpp
    pixel/time/frame_counter.h
    pixel/time/frame_rate_limiter.cpp
    pixel/time/frame_rate_limiter.h
    pixel/types.h
    pixel/util/collections.h
    pixel/util/symbol_map.cpp
    pixel/util/symbol_map.h
    pixel/util/util.cpp
    pixel/util/util.h
    pixel/renderers/renderer_group.h
    pixel/random.h
    pixel/time/simple_timer.cpp pixel/time/simple_timer.h
    pixel/physics/constraints.h
    pixel/physics/particle.h
    pixel/level.h
    pixel/tilemap/tile_map_collider.h
    pixel/objects/game_object.cpp pixel/objects/game_object.h
    pixel/objects/node.cpp pixel/objects/node.h
    pixel/imgui/gui.h pixel/imgui/gui.cpp
    )


IF (PIXEL_LUA_BINDING)
  list(APPEND pixel_sources
      pixel/lua/binding.cpp
      pixel/lua/wrap_camera.cpp
      pixel/lua/wrap_collision.cpp
      pixel/lua/wrap_glm.cpp
      pixel/lua/wrap_image_data.cpp
      pixel/lua/wrap_opengl.cpp
      pixel/lua/wrap_tileset.cpp
      pixel/lua/binding.h
      )
ENDIF (PIXEL_LUA_BINDING)

list(APPEND pixel_sources
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_demo.cpp
    pixel/imgui/imgui_impl_glfw.cpp
    pixel/imgui/imgui_impl_opengl3.cpp)

file(GLOB soloud_sources
    lib/soloud/src/core/*.cpp
    lib/soloud/src/audiosource/wav/*.c*
    lib/soloud/src/backend/alsa/soloud_alsa.cpp
    lib/soloud/src/backend/coreaudio/soloud_coreaudio.cpp)

ADD_LIBRARY(pixel ${pixel_sources} ${soloud_sources} pixel/audio/audio.cpp pixel/audio/audio.h pixel/animation/animation.cpp pixel/animation/animation.h pixel/animation/animation_group.cpp pixel/animation/animation_group.h pixel/animation/lambda_animation.cpp pixel/animation/lambda_animation.h pixel/renderers/textured_triangle_renderer.cpp pixel/renderers/textured_triangle_renderer.h pixel/input/input_device.cpp pixel/input/input_device.h pixel/input/input_keyboard.cpp pixel/input/input_keyboard.h pixel/input/input_button_base.cpp pixel/input/input_button_base.h)

target_compile_options(pixel PUBLIC -march=native)

if(APPLE)
  find_library(AUDIO_TOOLBOX CoreAudio)
  if (NOT AUDIO_TOOLBOX)
    message(FATAL_ERROR "AudioToolbox not found")
  endif()

  TARGET_LINK_LIBRARIES(
      pixel
      PUBLIC
      ${AUDIO_TOOLBOX}
  )

  TARGET_COMPILE_DEFINITIONS(pixel PRIVATE WITH_COREAUDIO)

elseif(UNIX AND NOT APPLE)
  find_package(ALSA REQUIRED)

  if (NOT ALSA_FOUND)
    message(FATAL_ERROR "Alsa not found")
  endif()

  message(${ALSA_LIBRARIES})
  message(${ALSA_INCLUDE_DIRS})
  message(${ALSA_INCLUDE_DIR})

  TARGET_LINK_LIBRARIES(
      pixel
      INTERFACE
      ${ALSA_LIBRARIES}
  )

    TARGET_INCLUDE_DIRECTORIES(pixel
        INTERFACE
        ${ALSA_INCLUDE_DIRS})

  TARGET_COMPILE_DEFINITIONS(pixel PUBLIC WITH_ALSA)
endif()

ADD_DEPENDENCIES(pixel pixel_source_version)

SET(TMXLITE_STATIC_LIB ON)
SET(GLM_TEST_ENABLE OFF)

ADD_SUBDIRECTORY(lib/glm)
ADD_SUBDIRECTORY(lib/sol2)
ADD_SUBDIRECTORY(lib/tmxlite/tmxlite)

TARGET_INCLUDE_DIRECTORIES(pixel PUBLIC . lib/imgui lib/soloud/include lib/glm lib/stb)

TARGET_LINK_LIBRARIES(
    pixel
    PUBLIC Boost::system Boost::filesystem glfw glew::glew tmxlite
    Lua::lua_lib
    sol2
)

IF (CMAKE_C_FLAGS_DEBUG)
  TARGET_COMPILE_DEFINITIONS(pixel PUBLIC PIXEL_DEBUG=1)
ELSE (CMAKE_C_FLAGS_DEBUG)
  TARGET_COMPILE_DEFINITIONS(pixel PUBLIC PIXEL_DEBUG=0)
ENDIF (CMAKE_C_FLAGS_DEBUG)

TARGET_COMPILE_DEFINITIONS(pixel PUBLIC _DEFAULT_SOURCE=1)
