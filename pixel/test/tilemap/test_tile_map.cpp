#include <pixel/pixel.h>
#include "test/setup.h"

namespace
{

using pixel::TileMap;

TEST_CASE("TileMap")
{
    pixel::TileMap tile_map({30, 20}, {16, 16});

    //ASSERT_TRUE(tile_map.load("assets/traps_1.tmx"));
};

};

