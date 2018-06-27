#include <pixel/collision/collision.h>
#include <test/setup.h>

namespace
{

using namespace pixel::collision;

TEST_CASE("CollisionMap")
{
    CollisionMap collision_map{30, 20};

    SECTION("set") {
        for (auto x = 0u; x < 30; ++x) {
            for (auto y = 0u; y < 20; ++y) {
                collision_map.set(x, y, (x + y) % 2);
            }

        }
    }

    SECTION("collide_row") {
        for (auto x = 0u; x < 10; ++x) {
            collision_map.set(x, 10, true);
        }

        REQUIRE(collision_map.collide_row(10, 0, 7));

        REQUIRE_FALSE(collision_map.collide_row(10, 10, 17));
    }

    SECTION("collide_column") {
        for (auto y = 0u; y < 10; ++y) {
            collision_map.set(10, y, true);
        }

        REQUIRE(collision_map.collide_column(10, 0, 7));

        REQUIRE_FALSE(collision_map.collide_column(10, 10, 17));
    }
}

};
