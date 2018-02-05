//
//

#include "tilemap.h"

using namespace pixel;

TileMap::TileMap(const tmx::Map& map, const tmx::TileLayer& layer)
{
    auto tile_count = map.getTileCount();
    _width = tile_count.x;
    _height = tile_count.y;
    _tiles.resize(_width * _height);
}
