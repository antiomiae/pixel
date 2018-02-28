
#include <pixel/pixel.h>
#include "texture_atlas.h"
#include "pack.h"

namespace pixel::graphics
{

void TextureAtlas::start_batch()
{
    blocks_.clear();
}


void TextureAtlas::stop_batch()
{
    std::sort(begin(blocks_), end(blocks_), [](auto& b1, auto& b2) { return max(b1.w, b1.h) > max(b2.w, b2.h); });
    auto res = pixel::pack::pack_rects_array(blocks_, texture_dimensions_.x, texture_dimensions_.y, texture_dimensions_.z);

    auto& packing = get<0>(res);
    auto& leftover = get<1>(res);

    error_if(leftover.size() > 0, "Images exceed size allocated for atlas after packing");

    ImageData temp_surface(texture_dimensions_.x, texture_dimensions_.y);

    sort(begin(packing), end(packing), [&] (const auto& a, const auto& b) { return get<1>(a).z < get<1>(b).z; });

    auto current_layer = 0u;
    for (auto& pack_pair : packing) {
        auto& image_size = get<0>(pack_pair);
        auto& pack_params = get<1>(pack_pair);

        // TODO need methods to copy data between ImageData, or replace with thirdparty class

        if (current_layer != pack_params.z) {
            // flush
        }
    }

    blocks_.clear();
    image_buffers_.clear();
}


uint32_t TextureAtlas::add_image(const std::string& path)
{
    const auto img_id = top_id_++;

    auto img = ImageData(path);

    blocks_.emplace_back(img.width, img.height, img_id);

    image_buffers_.insert({img_id, move(img)});

    return img_id;
}


TextureAtlas::TextureAtlas(glm::uvec3 size)
: texture_dimensions_(size)
{
}

};
