#include "setup.h"
#include <pixel/graphics/sprite.h>

namespace {

TEST(SpriteTest, PositionIsAUnion) {
pixel::graphics::Sprite sprite{};

sprite.x = 0;
sprite.y = 1;
sprite.z = 2;

EXPECT_TRUE(sprite.x == sprite.position[0]);
EXPECT_TRUE(sprite.y == sprite.position[1]);
EXPECT_TRUE(sprite.z == sprite.position[2]);
}

};

