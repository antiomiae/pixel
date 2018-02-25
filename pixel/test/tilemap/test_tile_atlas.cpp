#include "test/setup.h"

namespace
{

using pixel::TileAtlas;

TEST(TileAtlas, Constructor)
{
    TileAtlas tile_atlas{16, 16, 2560};

    EXPECT_EQ(16, tile_atlas.tile_width());
    EXPECT_EQ(16, tile_atlas.tile_height());
    EXPECT_EQ(2560, tile_atlas.max_tiles());

    EXPECT_EQ(16, tile_atlas.atlas_columns());
    EXPECT_EQ(16, tile_atlas.atlas_rows());
    EXPECT_EQ(10, tile_atlas.atlas_layers());
};

};
