#include <pixel/pixel.h>
#include "test/setup.h"

namespace
{

using pixel::TileMap;

TEST(TileMap, Constructor)
{
    pixel::TileMap tile_map({30, 20}, {16, 16});
    
    ASSERT_TRUE(tile_map.load("assets/traps_1.tmx"));
};

};

