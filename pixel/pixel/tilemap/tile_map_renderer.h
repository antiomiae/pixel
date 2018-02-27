#ifndef PIXEL_TILE_MAP_RENDERER_H
#define PIXEL_TILE_MAP_RENDERER_H

#include <string>
#include <pixel/pixel.h>
#include "tile_map.h"
#include "tile_layer_texture.h"

namespace pixel
{

using graphics::Shader;
using graphics::Buffer;
using graphics::Vao;

class TileMapRenderer
{
public:
    TileMapRenderer();
    void set_program(Shader&& p);
    void init();
    void render(pixel::TileMap& t, RenderContext projection);
    void set_buffer_data(float map_width, float map_height, float table_width, float table_height);
private:
    unique_ptr<Shader> program_;
    Buffer buffer_;
    Vao vao_;
    unique_ptr<TileLayerTexture> tile_layer_texture_;
};

};


#endif //PIXEL_TILE_MAP_RENDERER_H
