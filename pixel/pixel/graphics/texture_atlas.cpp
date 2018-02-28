

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

    blocks_.clear();
    image_buffers_.clear();
}


uint32_t TextureAtlas::add_image(const std::string& path)
{
    const auto img_id = top_id_++;

    auto img = ImageData(path);

    blocks_.emplace_back(img.width, img.height, img_id);

    image_buffers_.push_back(move(img));

    return img_id;
}


TextureAtlas::TextureAtlas(glm::uvec3 size)
: texture_dimensions_(size)
{
}

};
