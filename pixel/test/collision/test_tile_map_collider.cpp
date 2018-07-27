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
    crect.center = {8 + 16, 8 + 16};
    crect.delta = {1, 1};


    SECTION("nearest_tile_col") {
        REQUIRE(crect.nearest_tile_col() == 2);

        crect.delta = {-1, -1};

        REQUIRE(crect.nearest_tile_col() == 0);
    }

    SECTION("nearest_tile_row") {
        REQUIRE(crect.nearest_tile_row() == 2);

        crect.center = {8 + 13, 8 + 16};

        REQUIRE(crect.nearest_tile_row() == 2);

        crect.delta = {-1, -1};

        crect.center = {8 + 16, 8 + 16};

        REQUIRE(crect.nearest_tile_row() == 0);

        crect.center = {8 + 19, 8 + 16};

        REQUIRE(crect.nearest_tile_row() == 0);
    }

    SECTION("distance_to_tile_x") {
        crect.delta = {1, 1};

        REQUIRE(crect.distance_to_tile_x(2) == 0);
        REQUIRE(crect.distance_to_tile_x(0) == 0);

        crect.center.x -= 10;

        REQUIRE(crect.distance_to_tile_x(2) == 10);
        REQUIRE(crect.distance_to_tile_x(0) == -10);
    }
}

}

