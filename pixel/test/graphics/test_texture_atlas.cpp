#include "test/setup.h"
#include <pixel/graphics/texture_atlas.h>

namespace
{

using namespace pixel;
using namespace pixel::graphics;

TEST(TextureAtlas, batch)
{
    TextureAtlas atlas({512, 512, 6});

    atlas.start_batch();

    for (auto i = 0u; i < 100; ++i) {
        atlas.add_image("assets/random_images/" + to_string(i) + ".png");
    }

    atlas.stop_batch();

    auto& layers = atlas.layers();
    for (auto i = 0u; i < layers.size(); ++i) {
        auto path = "debug/tex_atlas_" + to_string(i) + ".png";
        layers[i].save(path);
    }

    cout << atlas.debug_print();
}

};

