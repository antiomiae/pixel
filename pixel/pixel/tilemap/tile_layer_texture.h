#ifndef PIXEL_TILE_LAYER_TEXTURE_H
#define PIXEL_TILE_LAYER_TEXTURE_H

#include "tile_layer.h"
#include "tile_atlas.h"
#include <pixel/graphics/graphics.h>

namespace pixel
{

using pixel::graphics::Texture;

class TileLayer;

/**
 * A texture of tile map data
 */
class TileLayerTexture
{
public:
    TileLayerTexture();
    void load(TileLayer& ref_layer, TileAtlas& atlas);
    Texture& texture();
private:
    unique_ptr<Texture> texture_;
    void init_texture();
    vector<uint16_t> tex_data_;

};

};
#endif //PIXEL_TILE_LAYER_TEXTURE_H
