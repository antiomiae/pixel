//
//

#ifndef PIXEL_TILESET_H
#define PIXEL_TILESET_H

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace pixel
{

struct Tile
{
    using TileId = uint16_t;

    struct Properties
    {
        uint32_t flags;
        uint8_t *height_map;
    };

    using PropertyMap = std::unordered_map<TileId, Properties>;

    TileId tile_id;
    uint16_t flags;
};

class TileMap
{

private:
    Tile::PropertyMap _props;
    std::vector<Tile> _tiles;
    unsigned int _width;
    unsigned int _height;

public:
    TileMap(unsigned int width, unsigned int height) : _width(width), _height(height), _tiles(width * height) {};

};

};

#endif //PIXEL_TILESET_H
