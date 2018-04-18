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

}
