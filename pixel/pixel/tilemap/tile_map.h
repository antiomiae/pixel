//
//

#ifndef PIXEL_TILE_MAP_H
#define PIXEL_TILE_MAP_H

#include <tmxlite/Map.hpp>
#include "tile_layer.h"
#include "tile_atlas.h"
#include <pixel/math/math.h>
#include <vector>

namespace pixel
{

class TileMap
{
private:
    unique_ptr<TileAtlas> _atlas;
    vector<TileLayer> _layers;
    glm::vec2 _tileSize;
    glm::vec2 _tileCount;

public:
    TileMap();

    bool load(const tmx::Map& map);

    TileAtlas& atlas() const;
    const vector<TileLayer>& layers() const;
    glm::vec2 tileCount() const;
    glm::vec2 tileSize() const;
};

};
#endif //PIXEL_TILE_MAP_H
