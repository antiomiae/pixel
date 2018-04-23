#include <pixel/collision/collision.h>
#include <test/setup.h>

namespace
{

using namespace pixel::collision;

TEST(CollisionMap, constructor)
{
    CollisionMap c{30, 20};

    EXPECT_TRUE(true);
}

TEST(CollisionMap, set)
{
    CollisionMap collision_map{30, 20};

    for (auto x = 0u; x < 30; ++x) {
        for (auto y = 0u; y < 20; ++y) {
            collision_map.set(x, y, (x + y) % 2);
        }

    }

    EXPECT_TRUE(true);
}

TEST(CollisionMap, collide_row)
{
    CollisionMap collision_map{30, 20};

    for (auto x = 0u; x < 10; ++x) {
        collision_map.set(x, 10, true);
    }

    EXPECT_TRUE(collision_map.collide_row(10, 0, 7));

    EXPECT_FALSE(collision_map.collide_row(10, 10, 17));
}

TEST(CollisionMap, collide_column)
{
    CollisionMap collision_map{30, 20};

    for (auto y = 0u; y < 10; ++y) {
        collision_map.set(10, y, true);
    }

    EXPECT_TRUE(collision_map.collide_column(10, 0, 7));

    EXPECT_FALSE(collision_map.collide_column(10, 10, 17));

}

}
