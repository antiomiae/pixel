#include "setup.h"
#include <pixel/graphics/sprite.h>

namespace {

using pixel::TileMap;

TEST(TileMap, ReferenceTile) {

    TileMap tilemap{10, 10};

    tilemap.at(0, 0).tile_id = 0x101;

    auto tile = ((const TileMap)tilemap).at(0, 0);

    ASSERT_TRUE(tile.tile_id == 0x101);
}

};

