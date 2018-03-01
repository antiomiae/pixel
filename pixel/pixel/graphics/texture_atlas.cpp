#include <pixel/pixel.h>
#include "texture_atlas.h"
#include "pack.h"
#include <sstream>

namespace pixel::graphics
{

void TextureAtlas::start_batch()
{
    blocks_.clear();
    layers_.clear();
    file_id_map_.clear();
    top_id_ = 0;
}

void TextureAtlas::process_batch()
{
    /* Blocks must be sorted from largest to smallest */
    std::sort(begin(blocks_), end(blocks_), [](auto& b1, auto& b2) { return max(b1.w, b1.h) > max(b2.w, b2.h); });

    auto res = pixel::pack::pack_rects_array(
        blocks_,
        tex_size_.x,
        tex_size_.y,
        tex_size_.z
    );

    auto& packing = res.first;
    auto& leftover = res.second;

    error_if(!leftover.empty(), "Images exceed size allocated for atlas after packing");

    sort(
        begin(packing),
        end(packing),
        [](const auto& a, const auto& b) {
            return a.second.z < b.second.z;
        }
    );

    ImageData temp_surface(tex_size_.x, tex_size_.y);

    auto current_layer = 0u;

    for (auto& pack_pair : packing) {
        auto& image_size = pack_pair.first;
        auto& pack_params = pack_pair.second;

        /* Encode position in atlas as TextureRegion */
        TextureRegion region;

        region.x = pack_params.x;
        region.y = pack_params.y;
        region.layer = pack_params.z;
        region.w = image_size.w;
        region.h = image_size.h;

        /* Associate region with unique region ID */
        tex_regions_[image_size.region_id] = region;

        error_if(current_layer > pack_params.z, "Layers out of order");

        /* Save current image as layer */
        if (current_layer != pack_params.z) {
            layers_.push_back(temp_surface);
            temp_surface.clear();
        }

        try {
            const auto& src_img = image_buffers_.at(image_size.region_id);
            temp_surface.load_subregion(src_img, 0, 0, src_img.width, src_img.height, pack_params.x, pack_params.y);
        } catch (exception& e) {
            cout << "Cannot find ImageData for region_id returned from packing function" << endl;
        }

        current_layer = pack_params.z;
    }

    /* Save last frame */
    layers_.push_back(temp_surface);
}


void TextureAtlas::stop_batch()
{
    process_batch();
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
    : tex_size_(size)
{
}

vector<ImageData>& TextureAtlas::layers()
{
    return layers_;
}

string print(const TextureRegion& r)
{
    stringstream out;

    out << "{ x = " << r.x
        << ", y = " << r.y
        << ", w = " << r.w
        << ", h = " << r.h
        << ", layer = " << r.layer
        << " }";

    return out.str();
}

vector<TextureAtlas::RegionMapItem> sort_regions(const TextureAtlas::RegionMap& r) {
    vector<TextureAtlas::RegionMapItem> out(r.size());

    std::copy(begin(r), end(r), begin(out));

    std::sort(begin(out), end(out),
              [](const auto& a, const auto& b) {
                  return a.first < b.first;
              }
    );

    return out;
};

string TextureAtlas::debug_print() const
{
    stringstream out;

    out << "TextureAtlas {" << endl
        << "  tex_size = { " << tex_size_.x << ", " << tex_size_.y << ", " << tex_size_.z << " }" << endl
        << "  tex_regions = { " << endl;

    for (auto & [k, v] : sort_regions(tex_regions_)) {
        out << "    " << k << " = " << print(v) << endl;
    }

    out << "  }" << endl

        << "}" << endl;

    return out.str();
}

};
