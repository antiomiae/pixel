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
#include "tile_atlas.h"

using namespace std;
using pixel::graphics::Texture;

namespace pixel
{

class TileLayer
{
public:
    struct Tile
    {
        uint16_t tile_id;
    };

    struct Properties
    {
        uint32_t flags;
        uint8_t* height_map;
    };

    using PropertyMap = std::unordered_map<uint16_t, Properties>;

    TileLayer() = default;
    TileLayer(const tmx::Map& m, const tmx::TileLayer& t, const pixel::TileAtlas& atlas);
    TileLayer(unsigned width, unsigned height);
    TileLayer(TileLayer&& rhs) noexcept;
    Texture& texture() const;
    void init();
    bool load(const tmx::Map& m, const tmx::TileLayer& t, const pixel::TileAtlas& atlas);


    Tile& at(unsigned int x, unsigned int y)
    {
        return _tiles.at(x + y * _height);
    }


    const Tile& at(unsigned int x, unsigned int y) const
    {
        return _tiles.at(x + y * _height);
    }


private:
    PropertyMap _props;
    std::vector<Tile> _tiles;
    unsigned int _width;
    unsigned int _height;
    unique_ptr<Texture> texture_;
};

};

#endif //PIXEL_TILESET_H
