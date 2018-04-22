//
//

#ifndef PIXEL_TILE_MAP_H
#define PIXEL_TILE_MAP_H

#include <vector>
#include <tmxlite/Map.hpp>
#include <pixel/math/math.h>
#include "tile_layer.h"
#include "tile_atlas.h"
#include "tileset.h"

namespace pixel
{

class TileMap
{

public:

    TileMap() = default;

    bool load(const tmx::Map& map);
    bool load(const std::string& tmx_path);

    TileAtlas& atlas();
    Tileset& tileset();
    vector<TileLayer>& layers();
    glm::uvec2 tile_count() const;
    glm::uvec2 tile_size() const;

    void update(float dt);

private:

    unique_ptr<TileAtlas> atlas_;
    Tileset tileset_;
    vector<TileLayer> layers_;
    glm::uvec2 tile_size_;
    glm::uvec2 tile_count_;

};

};

#endif //PIXEL_TILE_MAP_H
