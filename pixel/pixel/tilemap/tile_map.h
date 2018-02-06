//
//

#ifndef PIXEL_TILE_MAP_H
#define PIXEL_TILE_MAP_H

#include <tmxlite/Map.hpp>
#include "tile_layer.h"
#include "tile_atlas.h"

namespace pixel
{

class TileMap
{
private:
    unique_ptr<TileAtlas> _atlas;

public:
    TileMap();

    bool load(const tmx::Map& map);
};

};
#endif //PIXEL_TILE_MAP_H
