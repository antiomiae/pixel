#include "test/setup.h"
#include <pixel/lua/lua.h>

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
    }
};

TEST_F(BindingTest, TileMap)
{
    lua.script(
        R"(
local tile_map = pixel.TileMap.new()
assert(tile_map)

function pixel.load_map(p)
    tm = pixel.TileMap.new()
    tm:load(p)
    return tm
end

tile_map = pixel.load_map("assets/traps_1.tmx");
assert(tile_map)

)"
    );


};

};
