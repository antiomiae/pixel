//
//

#include <pixel/pixel.h>

using namespace pixel;
using namespace std;


TileLayer::TileLayer(unsigned width, unsigned height)
  : _width(width),
    _height(height),
    _tiles(width * height)
{
    init();
}


void TileLayer::init()
{
    _texture = make_unique<Texture>(GL_TEXTURE_2D, GL_RED_INTEGER, GL_R16UI, GL_UNSIGNED_SHORT);
    (*_texture).load(_width, _height, nullptr);
}


TileLayer::TileLayer(TileLayer&& rhs) noexcept
  : _texture(std::move(rhs._texture)),
    _width(rhs._width),
    _height(rhs._height),
    _tiles(move(rhs._tiles))
{
}


TileLayer::TileLayer(const tmx::Map& m, const tmx::TileLayer& t, const TileAtlas& atlas)
  : TileLayer(m.getTileCount().x, m.getTileCount().y)
{
    load(m, t, atlas);
}


bool TileLayer::load(const tmx::Map& m, const tmx::TileLayer& t, const pixel::TileAtlas& atlas)
{
    auto tmx_tiles = t.getTiles();

    error_if(tmx_tiles.size() != _tiles.size(), "tmx::TileLayer t contains different number of tiles than expected");

    transform(
      begin(tmx_tiles),
      end(tmx_tiles),
      begin(_tiles),
      [&](auto tmx_tile) {
          return Tile{atlas.atlasId(tmx_tile.ID)};
      }
    );

    (*_texture).loadSubregion(0, 0, _width, _height, _tiles.data());

    return true;
}
