//
//

#ifndef PIXEL_TILE_MAP_RENDERER_H
#define PIXEL_TILE_MAP_RENDERER_H

#include "tile_map.h"
#include <pixel/pixel.h>
#include <string>

namespace pixel
{

using graphics::Shader;
using graphics::Buffer;
using graphics::Vao;

class TileMapRenderer
{
private:
    unique_ptr<Shader> program_;
    Buffer buffer_;
    Vao vao_;
public:
    TileMapRenderer();
    void set_program(Shader&& p);
    void init();
    void render(pixel::TileMap& t, RenderContext projection);
    void set_buffer_data(float map_width, float map_height, float table_width, float table_height);
};

};


#endif //PIXEL_TILE_MAP_RENDERER_H
