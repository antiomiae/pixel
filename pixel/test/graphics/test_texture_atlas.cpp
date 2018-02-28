#include "test/setup.h"
#include <pixel/graphics/texture_atlas.h>

namespace
{

using namespace pixel;
using namespace pixel::graphics;

TEST(TextureAtlas, B)
{
    TextureAtlas atlas({1024, 1024, 6});

    atlas.start_batch();

    for (auto i = 0u; i < 100; ++i) {
        atlas.add_image("assets/random_images/" + to_string(i) + ".png");
    }

    atlas.stop_batch();
}

};

