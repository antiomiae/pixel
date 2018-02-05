//
//

#ifndef PIXEL_TILESET_H
#define PIXEL_TILESET_H

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "../graphics/texture.h"

using namespace std;
using pixel::graphics::Texture;

namespace pixel
{

class TileMap
{
    using TileId = uint16_t;

    struct Tile
    {
        TileId tile_id;
    };

    struct Properties
    {
        uint32_t flags;
        uint8_t* height_map;
    };

    using PropertyMap = std::unordered_map<TileId, Properties>;

private:
    PropertyMap _props;
    std::vector<Tile> _tiles;
    unsigned int _width;
    unsigned int _height;
    Texture _tex;

public:
    TileMap(const tmx::Map& map, const tmx::TileLayer& layer);

    Tile& at(unsigned int x, unsigned int y)
    {
        return _tiles.at(x + y * _height);
    }

    const Tile& at(unsigned int x, unsigned int y) const
    {
        return _tiles.at(x + y * _height);
    }
};

};

#endif //PIXEL_TILESET_H
