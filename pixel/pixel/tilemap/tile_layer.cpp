//
//

#include "tile_layer.h"

using namespace pixel;

TileLayer::TileLayer(unsigned width, unsigned height)
  : _width(width),
    _height(height),
    _tiles(width * height)
{
    init();
}

void TileLayer::init()
{
    _texture = make_unique<Texture>(GL_TEXTURE_2D, GL_RED_INTEGER, GL_UNSIGNED_SHORT);
    (*_texture).load(_width, _height, nullptr);
}

TileLayer::TileLayer(TileLayer&& rhs) noexcept
  : _texture(std::move(rhs._texture)),
    _width(rhs._width),
    _height(rhs._height),
    _tiles(rhs._width * rhs._height)
{
}
