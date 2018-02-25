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
    const std::vector<Tile>& tiles() const;
    std::vector<Tile>& tiles();
    void init();
    bool load(const tmx::Map& m, const tmx::TileLayer& t, const pixel::TileAtlas& atlas);


    Tile& at(unsigned int x, unsigned int y)
    {
        return tiles_.at(x + y * height_);
    }


    const Tile& at(unsigned int x, unsigned int y) const
    {
        return tiles_.at(x + y * height_);
    }


private:
    PropertyMap _props;
    std::vector<Tile> tiles_;
    unsigned int width_;
    unsigned int height_;
    unique_ptr<Texture> texture_;
};

};

#endif //PIXEL_TILESET_H
