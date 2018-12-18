#include <pixel/graphics/graphics.h>
#include "test/setup.h"

namespace
{

using namespace pixel;
using namespace pixel::graphics;

TEST_CASE("TextureAtlas")
{
    TextureAtlas atlas({1024, 1024, 20});

    atlas.start_batch();

    for (auto i = 0u; i < 250; ++i) {
        atlas.add_image("assets/random_images/" + to_string(i) + ".png");
    }

    ImageData img{32, 32};
    atlas.add_image(img, "ImageData image");

    atlas.stop_batch();

    auto texture = atlas.as_texture();

    int i = 0;
    for (auto& img : atlas.layers()) {
        img.save("atlas_"s + to_string(i) + ".png"s);
        i++;
    }


}

};

