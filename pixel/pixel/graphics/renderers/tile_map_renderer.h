#ifndef PIXEL_TILE_MAP_RENDERER_H
#define PIXEL_TILE_MAP_RENDERER_H

#include <string>
#include <pixel/graphics/graphics.h>
#include "pixel/tilemap/tile_map.h"
#include "pixel/tilemap/tile_layer_texture.h"


namespace pixel
{
class TileLayerTexture;
class TileMap;
class TileAtlas;
}

namespace pixel::graphics::renderers
{

using graphics::Shader;
using graphics::Buffer;
using graphics::Vao;
using graphics::Camera;

class TileMapRenderer
{

public:

    TileMapRenderer();

    void render(pixel::TileMap& t, Camera& camera);

private:

    Shader program_;
    Buffer buffer_;
    Vao vao_;
    unique_ptr<TileLayerTexture> tile_layer_texture_;

    void init();
    void set_buffer_data(float map_width, float map_height, float table_width, float table_height);
};

};


#endif //PIXEL_TILE_MAP_RENDERER_H
