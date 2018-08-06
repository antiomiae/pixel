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

bool overlapping_solid_tiles(TileLayer& tile_layer, const CollisionRect& rect)
{
    auto tile_count = tile_layer.parent().tile_count();

    bool found_solid_tile = false;
    int minx = rect.center.x - rect.half_size.x;
    int miny = rect.center.y - rect.half_size.y;
    int maxx = ceil(rect.center.x + rect.half_size.x - 1);
    int maxy = ceil(rect.center.y + rect.half_size.y - 1);

    tile_layer.visit_tiles(
        {
            max(minx / 16, 0),
            max(miny / 16, 0)
        },
        {
            min(maxx / 16, (int)tile_count.x - 1),
            min(maxy / 16, (int)tile_count.y - 1)
        },

        [&] (auto tile_coord, auto& tile) -> bool {
            if (tile.tile_id != 0) {
                auto& tile_desc = tile_layer.parent().tileset().tile(tile.tile_id);

                if (tile_desc.type == "brick") {
                    found_solid_tile = true;
                    return false;
                }
            }

            return true;
        }
    );

    return found_solid_tile;
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

        CollisionRect rect;
        rect.center = {16 * 3 + 5, 16 * 4 + 5};
        rect.half_size = {5, 5};
        rect.tile_size = {16, 16};

        rect.delta = {16 * 2, 0};

        auto end_position = rect.center + rect.delta;

        auto collision_axes = glm::ivec2();
        auto center_after_collision = glm::vec2();

        tie(collision_axes, center_after_collision) = TileMapCollider().collide(
            rect,
            tile_layer,
            [](auto& tile, auto& tile_desc) -> auto {
                return tile_desc.type == "brick";
            },
            false
        );

        REQUIRE(collision_axes == glm::ivec2{0, 0});
        REQUIRE(center_after_collision.x == end_position.x);
        REQUIRE(center_after_collision.y == end_position.y);
        REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));


        end_position = rect.center + glm::vec2(0, -16);
        rect.delta = end_position - rect.center;

        tie(collision_axes, center_after_collision) = TileMapCollider().collide(
            rect,
            tile_layer,
            [](auto& tile, auto& tile_desc) -> auto {
                return tile_desc.type == "brick";
            }, false
        );

        REQUIRE(collision_axes.y == -1);
        REQUIRE(center_after_collision.x == end_position.x);
        REQUIRE(center_after_collision.y == rect.center.y);
        REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));


        rect.center.x = 10 * 16 + 5;

        end_position = rect.center + glm::vec2(0, -16);
        rect.delta = end_position - rect.center;

        tie(collision_axes, center_after_collision) = TileMapCollider().collide(
            rect,
            tile_layer,
            [](auto& tile, auto& tile_desc) -> auto {
                return tile_desc.type == "brick";
            }, false
        );

        REQUIRE(collision_axes.y == 0);
        REQUIRE(center_after_collision.x == end_position.x);
        REQUIRE(center_after_collision.y == end_position.y);
        REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));


        end_position = rect.center + glm::vec2(16, -16);
        rect.delta = end_position - rect.center;

        tie(collision_axes, center_after_collision) = TileMapCollider().collide(
            rect,
            tile_layer,
            [](auto& tile, auto& tile_desc) -> auto {
                return tile_desc.type == "brick";
            }, false
        );

        REQUIRE(collision_axes.y == 0);
        REQUIRE(center_after_collision.x == end_position.x);
        REQUIRE(center_after_collision.y == end_position.y);
        REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));



        rect.center = {48 + 5, 159.75 + 5};
        end_position = {48 + 5, 159.5 + 5};
        rect.delta = end_position - rect.center;

        // tile 9: 144 - 160

        tie(collision_axes, center_after_collision) = TileMapCollider().collide(
            rect,
            tile_layer,
            [](auto& tile, auto& tile_desc) -> auto {
                return tile_desc.type == "brick";
            }, false
        );

        REQUIRE(collision_axes.y == 0);
        REQUIRE(center_after_collision.x == end_position.x);
        REQUIRE(center_after_collision.y == end_position.y);
        REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));



        rect.center = {48 + 5, 159.75 + 5};
        end_position = {48 + 16 + 5, 159.75 + 30 + 5};
        rect.delta = end_position - rect.center;

        // tile 9: 144 - 160

        tie(collision_axes, center_after_collision) = TileMapCollider().collide(
            rect,
            tile_layer,
            [](auto& tile, auto& tile_desc) -> auto {
                return tile_desc.type == "brick";
            }, false
        );

        REQUIRE(collision_axes.y == 0);
        REQUIRE(center_after_collision.x == end_position.x);
        REQUIRE(center_after_collision.y == end_position.y);
        REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));

        SECTION("real-world case 1") {
            auto rect = CollisionRect(
                glm::vec2(200.500000, 21.000000), // 12.53, 1.3125
                glm::vec2(5.000000, 5.000000),
                glm::vec2(-4.000000, -5.000000),
                glm::ivec2(16, 16)
            );

            end_position = rect.center + rect.delta;

            REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));

            tie(collision_axes, center_after_collision) = TileMapCollider().collide(
                rect,
                tile_layer,
                [](auto& tile, auto& tile_desc) -> auto {
                    return tile_desc.type == "brick";
                }, true
            );

            REQUIRE(center_after_collision.x == end_position.x);
            REQUIRE(center_after_collision.y == 21);

            REQUIRE(collision_axes.x == 0);
            REQUIRE(collision_axes.y == -1);

            REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));
        }

        SECTION("corner heading nw") {
            auto rect = CollisionRect(
                glm::vec2(10*16 + 8, 2*16 + 8),
                glm::vec2(5.000000, 5.000000),
                glm::vec2(-10, 10.000000),
                glm::ivec2(16, 16)
            );

            end_position = {10*16 + 5, rect.center.y + 10};

            REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));

            tie(collision_axes, center_after_collision) =
                TileMapCollider().collide(
                rect,
                tile_layer,
                [](auto& tile, auto& tile_desc) -> auto {
                    return tile_desc.type == "brick";
                },
                true
            );


            REQUIRE(collision_axes.x == -1);
            REQUIRE(collision_axes.y == 0);
            REQUIRE(center_after_collision.x == end_position.x);
            REQUIRE(center_after_collision.y == end_position.y);

            REQUIRE_FALSE(overlapping_solid_tiles(tile_layer, rect));
        }

        // 10, 2

    }
}

}

