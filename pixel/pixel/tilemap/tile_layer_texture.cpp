
#include <algorithm>
#include <pixel/error.h>
#include "tile_layer_texture.h"


namespace pixel
{

void TileLayerTexture::init_texture()
{
    texture_ = make_unique<Texture>(GL_TEXTURE_2D, GL_RED_INTEGER, GL_R16UI, GL_UNSIGNED_SHORT);
    texture_->load(0, 0);
}


void TileLayerTexture::load(pixel::TileLayer& ref_layer, pixel::TileAtlas& atlas)
{
    glm::uvec2 min_size = {
        max(ref_layer.width(), 8u),
        max(ref_layer.height(), 8u)
    };

    if (tex_data_.size() != min_size.x * min_size.y) {
        tex_data_.resize(min_size.x * min_size.y);
    }

    for (auto y = 0u; y < ref_layer.height(); ++y) {
        for (auto x = 0u; x < ref_layer.width(); ++x) {
            auto& tile = ref_layer.tiles()[x + y*ref_layer.width()];
            tex_data_[x + (min_size.x * y)] = atlas.atlas_id_from_tmx_id(tile.tile_id, tile.flip_flags);
        }
    }

    (*texture_).load(min_size.x, min_size.y, (uint8_t*)tex_data_.data());
    log_gl_errors();
}


TileLayerTexture::TileLayerTexture()
{
    init_texture();
}


Texture& TileLayerTexture::texture()
{
    return *texture_;
}

};
