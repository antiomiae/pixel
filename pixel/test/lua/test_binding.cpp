#if PIXEL_LUA_BINDING


#include "test/setup.h"
#include <pixel/lua/binding.h>

namespace
{

TEST_CASE("lua binding")
{
    sol::state lua;

    lua.open_libraries();
    pixel::binding::bind_pixel(lua);
    lua.script("package.path = package.path .. ';pixel/lua/?.lua'");
    lua.script("require 'pixel'");
    lua["app"] = pixel::app();

    SECTION("TileMap")
    {
        lua.script(R"(
        local tile_map = pixel.TileMap.new(pixel.glm.uvec2.new(30, 20), pixel.glm.uvec2.new(16, 16))
        assert(tile_map)

        tile_map = pixel.TileMap.from_path('assets/traps_1.tmx')
        assert(tile_map)
    )");
    }

    SECTION("SpriteRenderer")
    {
        lua.script(R"(
        local sprite_renderer = pixel.SpriteRenderer.new()
        assert(sprite_renderer)
    )");
    }

    SECTION("SpriteAnimation")
    {
        lua.script(R"(
        local anim = pixel.SpriteAnimation.new()
        assert(anim)
    )");
    }

    SECTION("Camera")
    {
        lua.script(R"(
        local camera = pixel.Camera.new()
        assert(camera)
    )");
    }

    SECTION("Keyboard")
    {
        lua.script(R"(
        assert(pixel.Keyboard.keymap)

        pixel.Keyboard.register_callback(app:window())

        local is_a_down = pixel.Keyboard.keymap[string.byte("A")]
        assert(is_a_down ~= nil)
    )");
    }

    SECTION("ImageData")
    {
        lua.script(R"(
        assert(pixel.ImageData)

        local image1 = pixel.ImageData.new(100, 100)
        local image2 = pixel.ImageData.new(image1)
        local image3 = image2:transpose()
        image3:clear()

        local num_pixels = image1.width * image1.height
        assert(num_pixels == 100 * 100)
    )");
    }
};

};


#endif
