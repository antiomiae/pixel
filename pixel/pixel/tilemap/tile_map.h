#ifndef PIXEL_TILE_MAP_H
#define PIXEL_TILE_MAP_H

#include <vector>
#include <memory>
#include <tmxlite/Map.hpp>
#include <pixel/math.h>
#include "tile_atlas.h"
#include "tileset.h"

namespace pixel
{

using namespace std;

class TileLayer;
class TileAtlas;

struct TileCoordinate
{
    int x{0}, y{0};

    TileCoordinate() = default;

    TileCoordinate(int x, int y)
        : x{x},
          y{y}
    { };
};


class TileMap
{

public:

    TileMap(const glm::uvec2& map_size, const glm::uvec2& tile_size);

    bool load(const tmx::Map& map);
    //bool load(const std::string& tmx_path);

    static unique_ptr<TileMap> from_path(const string& tmx_path);

    TileAtlas& atlas();
    Tileset& tileset();
    vector<pixel::TileLayer>& layers();

    glm::uvec2 tile_count() const;

    glm::uvec2 tile_size() const;

    glm::vec4 background_color() const;

    void update(float dt);


private:

    unique_ptr<TileAtlas> atlas_;
    Tileset tileset_;
    vector<pixel::TileLayer> layers_;
    glm::uvec2 tile_size_;
    glm::uvec2 tile_count_;

    glm::vec4 background_color_;

};

};

#endif //PIXEL_TILE_MAP_H
