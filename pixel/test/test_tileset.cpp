#include "setup.h"
#include <pixel/tilemap/tile_atlas.h>

namespace
{

using pixel::TileAtlas;

TEST(TileAtlas, Constructor)
{
    TileAtlas tile_atlas{16, 16, 2560};

    EXPECT_EQ(16, tile_atlas.tileWidth());
    EXPECT_EQ(16, tile_atlas.tileHeight());
    EXPECT_EQ(2560, tile_atlas.maxTiles());

    EXPECT_EQ(16, tile_atlas.atlasColumns());
    EXPECT_EQ(16, tile_atlas.atlasRows());
    EXPECT_EQ(10, tile_atlas.atlasLayers());
};

};
