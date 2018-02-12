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
    unique_ptr<Shader> _program;
    Buffer _buffer;
    Vao _vao;
public:
    TileMapRenderer();
    explicit TileMapRenderer(Shader&& p);
    void setProgram(Shader&& p);
    void init();
    void render(TileMap& t);
    void setBufferData(float w, float h, float tw, float th);
};

};


#endif //PIXEL_TILE_MAP_RENDERER_H
