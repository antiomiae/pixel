#include "test/setup.h"
#include <pixel/lua/binding.h>

namespace
{

class BindingTest : public ::testing::Test
{
public:
    sol::state lua;

    BindingTest()
    {
        lua.open_libraries();
        pixel::binding::bind_pixel(lua);
        lua.script("package.path = package.path .. ';pixel/lua/?.lua'");
        lua.script("require 'pixel'");
        lua["app"] = pixeltest::app();
    }
};

TEST_F(BindingTest, TileMap)
{
    lua.script(R"(
        local tile_map = pixel.TileMap.new()
        assert(tile_map)

        tile_map = pixel.load_map("assets/traps_1.tmx");
        assert(tile_map)

    )");
}

TEST_F(BindingTest, SpriteRenderer)
{
    lua.script(R"(
        local sprite_renderer = pixel.SpriteRenderer.new()
        assert(sprite_renderer)
    )");
};

TEST_F(BindingTest, SpriteAnimation)
{
    lua.script(R"(
        local anim = pixel.SpriteAnimation.new()
        assert(anim)
    )");
};


TEST_F(BindingTest, Camera)
{
    lua.script(R"(
        local camera = pixel.Camera.new()
        assert(camera)
    )");
};

TEST_F(BindingTest, Keyboard)
{
    lua.script(R"(
        assert(pixel.Keyboard.keymap)

        pixel.Keyboard.register_callback(app:window())

        local is_a_down = pixel.Keyboard.keymap[string.byte("A")]
        assert(is_a_down ~= nil)
)");
};

TEST_F(BindingTest, ImageData)
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
};

};
