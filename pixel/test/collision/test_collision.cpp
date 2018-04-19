#include <pixel/collision/collision.h>
#include <test/setup.h>

namespace
{

using namespace pixel::collision;

TEST(CollisionMap, constructor)
{
    CollisionMap c1{};
    CollisionMap c2{30, 20};

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

}
