#include "test/setup.h"

namespace
{

using pixel::TileAtlas;

TEST_CASE("TileAtlas")
{
    TileAtlas tile_atlas{16, 16, 2560};

    REQUIRE(16 == tile_atlas.tile_width());
    REQUIRE(16 == tile_atlas.tile_height());
    REQUIRE(2560 == tile_atlas.max_tiles());

    REQUIRE(16 == tile_atlas.atlas_columns());
    REQUIRE(16 == tile_atlas.atlas_rows());
    REQUIRE(10 == tile_atlas.atlas_layers());
};

};
