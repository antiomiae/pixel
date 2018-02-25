//
//

#ifndef PIXEL_TILE_LAYER_H
#define PIXEL_TILE_LAYER_H

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

/**
 * Container of tile map data. Logically, a rectangular array of tile map tiles, regularly arranged and identically
 * sized objects that are drawn as a contiguous image, and which may carry additional information to be used in
 * collision detection or pathfinding.
 *
 * Maintains a collection of TileLayer::Tile and a gpu texture with data necessary to render the layer.
 */
class TileLayer
{
public:
    /**
     * Tile map data.
     * */
    struct Tile
    {
        /**
         * Tile orientation flags
         * */
        enum FlipFlag
        {
            kHorizontal = 0x8,
            kVertical = 0x4,
            kDiagonal = 0x2
        };

        /**
         * ID of tile in pixel::Tileset
         */
        uint32_t tile_id{};
        /*
         * Orientation of tile, according to `Tile::FlipFlag`
         */
        uint8_t flip_flags{};
        /*
         * ID that encodes the `pixel::TileAtlas` ID for this tile's texture atlas section
         * along with `flip_flags`.
         */
        uint16_t atlas_id{};

        Tile() = default;

        Tile(uint32_t tile_id, uint8_t flip_flags, uint16_t atlas_id)
            : tile_id{tile_id},
              flip_flags{flip_flags},
              atlas_id{atlas_id}
        { }
    };

    struct Properties
    {
        uint32_t flags;
        uint8_t* height_map;
    };

    struct TileAnimation
    {
        /**
         * Describes animation frames and durations
         */
        tmx::Tileset::Tile::Animation animation_definition;
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
    void update_texture();
};

};

#endif //PIXEL_TILE_LAYER_H
