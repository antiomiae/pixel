#include <pixel/graphics/graphics.h>
#include "test/setup.h"

namespace
{

using namespace pixel;
using namespace pixel::graphics;

TEST_CASE("TextureAtlas")
{
    TextureAtlas atlas({512, 512, 5});

    atlas.start_batch();

    for (auto i = 0u; i < 50; ++i) {
        atlas.add_image("assets/random_images/" + to_string(i) + ".png");
    }

    ImageData img{32, 32};
    atlas.add_image(img, "ImageData image");

    atlas.stop_batch();

    auto texture = atlas.as_texture();
}

};

