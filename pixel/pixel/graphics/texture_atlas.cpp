#include <pixel/pixel.h>
#include "texture_atlas.h"
#include "pack.h"

namespace pixel::graphics
{

void TextureAtlas::start_batch()
{
    blocks_.clear();
    layers_.clear();
    file_id_map_.clear();
    top_id_ = 0;
}


void TextureAtlas::stop_batch()
{
    /* Blocks must be sorted from largest to smallest */
    std::sort(begin(blocks_), end(blocks_), [](auto& b1, auto& b2) { return max(b1.w, b1.h) > max(b2.w, b2.h); });

    auto res = pixel::pack::pack_rects_array(blocks_,
                                             texture_dimensions_.x,
                                             texture_dimensions_.y,
                                             texture_dimensions_.z);

    auto& packing = get<0>(res);
    auto& leftover = get<1>(res);

    error_if(!leftover.empty(), "Images exceed size allocated for atlas after packing");

    sort(begin(packing), end(packing), [&](const auto& a, const auto& b) { return get<1>(a).z < get<1>(b).z; });

    ImageData temp_surface(texture_dimensions_.x, texture_dimensions_.y);

    auto current_layer = 0u;

    for (auto& pack_pair : packing) {
        auto& image_size = get<0>(pack_pair);
        auto& pack_params = get<1>(pack_pair);

        /* Encode position in atlas as TextureRegion */
        TextureRegion region;

        region.x = pack_params.x;
        region.y = pack_params.y;
        region.layer = pack_params.z;
        region.w = image_size.w;
        region.h = image_size.h;

        /* Associate region with unique region ID */
        tex_regions_[image_size.region_id] = region;

        /* Save current image as layer */
        if (current_layer != pack_params.z) {
            layers_.push_back(temp_surface);
            temp_surface.clear();
        }

        const auto& src_img = image_buffers_.at(image_size.region_id);
        temp_surface.load_subregion(src_img, 0, 0, src_img.width, src_img.height, pack_params.x, pack_params.y);

        current_layer = pack_params.z;
    }

    /* Save last frame */
    layers_.push_back(temp_surface);

//    blocks_.clear();
//
//    image_buffers_.clear();
}


uint32_t TextureAtlas::add_image(const std::string& path)
{
    const auto img_id = top_id_++;
    auto img = ImageData(path);

    file_id_map_[path] = img_id;
    blocks_.emplace_back(img.width, img.height, img_id);
    image_buffers_.insert(make_pair(img_id, move(img)));

    return img_id;
}


TextureAtlas::TextureAtlas(glm::uvec3 size)
        : texture_dimensions_(size)
{
}

vector<ImageData>& TextureAtlas::layers()
{
    return layers_;
}

};
