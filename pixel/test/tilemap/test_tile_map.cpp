#include <pixel/pixel.h>
#include "test/setup.h"

namespace
{

using pixel::TileMap;

TEST(TileMap, Constructor)
{
    pixel::TileMap tile_map;
    tile_map.load("assets/traps_1.tmx");
};

};

