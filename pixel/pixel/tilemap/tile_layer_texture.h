#ifndef PIXEL_TILE_LAYER_TEXTURE_H
#define PIXEL_TILE_LAYER_TEXTURE_H

#include <pixel/graphics/graphics.h>
#include "tile_layer.h"
#include "tile_atlas.h"

namespace pixel
{

using pixel::graphics::Texture;

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

};

};
#endif //PIXEL_TILE_LAYER_TEXTURE_H
