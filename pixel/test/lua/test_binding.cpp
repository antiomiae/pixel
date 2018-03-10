#include "test/setup.h"
#include <pixel/lua/lua_binding.h>

namespace
{

class BindingTest : public ::testing::Test
{
public:
    sol::state lua;

    BindingTest()
    {
        lua.open_libraries();
        pixel::bind_pixel(lua);
        lua.script("package.path = package.path .. ';pixel/lua/?.lua'");
        lua.script("require 'pixel'");
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

};
