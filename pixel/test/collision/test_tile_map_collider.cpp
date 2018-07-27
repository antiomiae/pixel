#include <test/setup.h>
#include <pixel/tilemap/tile_map_collider.h>

namespace
{

using namespace pixel;

TEST_CASE("CollisionRect2")
{
    CollisionRect2 crect;
    crect.tile_size = {16, 16};
    crect.half_size = {8, 8};
    crect.center = {8+16, 8+16};

    SECTION("nearest_tile_col") {
        crect.delta = {1,1};

        REQUIRE(crect.nearest_tile_col() == 2);

        crect.delta = {-1, -1};

        REQUIRE(crect.nearest_tile_col() == 0);
    }

    SECTION("nearest_tile_row") {
        crect.delta = {1,1};

        REQUIRE(crect.nearest_tile_row() == 2);

        crect.delta = {-1,-1};

        REQUIRE(crect.nearest_tile_row() == 0);
    }
}
}

