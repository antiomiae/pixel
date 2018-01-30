//
//

#ifndef PIXEL_TILESET_H
#define PIXEL_TILESET_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <iostream>
using namespace std;

namespace pixel
{

class TileMap
{
    using TileId = uint16_t;

    struct Tile
    {
        TileId tile_id;
        uint16_t flags;
    };

    struct Properties
    {
        uint32_t flags;
        uint8_t *height_map;
    };

    using PropertyMap = std::unordered_map<TileId, Properties>;

private:
    PropertyMap _props;
    std::vector<Tile> _tiles;
    unsigned int _width;
    unsigned int _height;

public:
    TileMap(unsigned int width, unsigned int height) : _width(width), _height(height), _tiles(width * height)
    {};

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
