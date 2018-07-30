#include <test/setup.h>
#include <pixel/tilemap/tile_map_collider.h>

namespace
{

using namespace pixel;

TEST_CASE("CollisionRect")
{
    CollisionRect crect;
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

        crect.center.x = 24 - 10;
        REQUIRE(crect.distance_to_tile_x(2) == 10);
        REQUIRE(crect.distance_to_tile_x(3) == 10+16);
        REQUIRE(crect.distance_to_tile_x(0) == 10);

        crect.center.x = 24 + 10;
        REQUIRE(crect.distance_to_tile_x(0) == -10);
        REQUIRE(crect.distance_to_tile_x(2) == -10);
        REQUIRE(crect.distance_to_tile_x(3) == -10 + 16);
    }

    SECTION("distance_to_tile_y") {
        crect.delta = {1, 1};

        REQUIRE(crect.distance_to_tile_y(2) == 0);
        REQUIRE(crect.distance_to_tile_y(0) == 0);

        crect.center.y = 24 - 10;
        REQUIRE(crect.distance_to_tile_y(2) == 10);
        REQUIRE(crect.distance_to_tile_y(3) == 10+16);
        REQUIRE(crect.distance_to_tile_y(0) == 10);

        crect.center.y = 24 + 10;
        REQUIRE(crect.distance_to_tile_y(0) == -10);
        REQUIRE(crect.distance_to_tile_y(2) == -10);
        REQUIRE(crect.distance_to_tile_y(3) == -10 + 16);
    }

    SECTION("intersection_time_x") {
        REQUIRE(crect.intersection_time_x(2) == 0);
        REQUIRE(crect.intersection_time_x(3) == 16);

        crect.center.x = 8 + 16 * 2;
        REQUIRE(crect.intersection_time_x(0) == -16);
    }

    SECTION("intersection_time_y") {
        REQUIRE(crect.intersection_time_y(2) == 0);
        REQUIRE(crect.intersection_time_y(3) == 16);

        crect.center.y = 8 + 16 * 2;
        REQUIRE(crect.intersection_time_y(0) == -16);
    }

    /*

     */
}

TEST_CASE("TileMapCollider")
{
    auto tile_map = TileMap::from_path("assets/map2.tmx");
    auto& tile_layer = tile_map->layers()[0];

    SECTION("collide") {

//        static glm::ivec2 collide(
//            BoundingBox& object,
//            TileLayer& tile_layer,
//            const function<bool(TileLayer::Tile & , Tileset::Tile & )>& tile_callback,
//            bool slide = false
//        )

        BoundingBox muh_box;

        muh_box.start_position = {16 * 3, 16 * 4};
        muh_box.size = {10, 10};

        auto end_position = muh_box.start_position + glm::vec2(16 * 2, 0);

        muh_box.end_position = end_position;

        auto collision_axes = TileMapCollider::collide(
            muh_box,
            tile_layer,
            [](auto& tile, auto& tile_desc) -> auto {
                return tile_desc.type == "brick";
            }, false
        );

        REQUIRE(muh_box.end_position.x == end_position.x);
        REQUIRE(muh_box.end_position.y == end_position.y);

        end_position = muh_box.start_position + glm::vec2(0, -16);
        muh_box.end_position = end_position;

        collision_axes = TileMapCollider::collide(
            muh_box,
            tile_layer,
            [](auto& tile, auto& tile_desc) -> auto {
                return tile_desc.type == "brick";
            }, false
        );

        REQUIRE(collision_axes.y == -1);
        REQUIRE(muh_box.end_position.x == end_position.x);
        REQUIRE(muh_box.end_position.y == muh_box.start_position.y);


    }
}

}

