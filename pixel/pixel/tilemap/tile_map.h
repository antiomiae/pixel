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
    unique_ptr<TileAtlas> atlas_;
    vector<TileLayer> layers_;
    glm::vec2 tile_size_;
    glm::vec2 tile_count_;

public:
    TileMap() = default;

    bool load(const tmx::Map& map);

    TileAtlas& atlas() const;
    const vector<TileLayer>& layers() const;
    glm::vec2 tile_count() const;
    glm::vec2 tile_size() const;
};

};
#endif //PIXEL_TILE_MAP_H
