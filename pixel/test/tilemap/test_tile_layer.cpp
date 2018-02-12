#include "test/setup.h"

namespace
{

using pixel::TileLayer;

TEST(TileLayer, ReferenceTile)
{

    TileLayer t{10, 10};

    t.at(0, 0).tile_id = 0x101;

    auto tile = t.at(0, 0);

    ASSERT_TRUE(tile.tile_id == 0x101);
};

};

