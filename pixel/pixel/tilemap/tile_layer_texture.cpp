
#include <pixel/pixel.h>
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
    vector<uint16_t> tex_data(ref_layer.tiles().size());

    std::transform(
        begin(ref_layer.tiles()),
        end(ref_layer.tiles()),
        begin(tex_data),
        [&](auto& tile) { return atlas.atlas_id_from_tmx_id(tile.tile_id, tile.flip_flags); }
    );

    (*texture_).load(ref_layer.width(), ref_layer.height(), (uint8_t*)tex_data.data());
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
