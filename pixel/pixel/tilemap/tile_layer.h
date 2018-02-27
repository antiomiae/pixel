//
//

#ifndef PIXEL_TILE_LAYER_H
#define PIXEL_TILE_LAYER_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <pixel/graphics/graphics.h>
#include "tileset.h"

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

        Tile() = default;


        Tile(uint32_t tile_id, uint8_t flip_flags)
            : tile_id{tile_id},
              flip_flags{flip_flags}
        { }
    };

    struct Properties
    {
        uint32_t flags;
        uint8_t* height_map;
    };

    class TileAnimation
    {
    public:
        TileAnimation(uint32_t id, pixel::Tileset::Tile::Animation animation)
            : base_tile_id{id},
              animation_definition{animation}
        {
        }


        TileAnimation() = default;

        /**
         * Describes animation frames and durations
         */
        Tileset::Tile::Animation animation_definition;
        /**
         * Indices of tiles that have this animation
         */
        vector<unsigned> tiles;
        /**
         * ID of tile this animation is derived from
         */
        uint32_t base_tile_id;
        /**
         * Timer
         */
        float timer;
        /**
         * Current frame in sequence
         */
        unsigned frame;


        void update(float dt)
        {
            timer += dt * 1000;
            update_frame();
        }


        void update_frame()
        {
            while (timer >= animation_definition.frames[frame].duration) {
                timer -= animation_definition.frames[frame].duration;
                frame++;
                if (frame >= animation_definition.frames.size()) {
                    frame = 0;
                }
            }
        }

        uint32_t tile_id()
        {
            return animation_definition.frames[frame].tile_id;
        }
    };

    using PropertyMap = std::unordered_map<uint16_t, Properties>;

    TileLayer() = default;
    TileLayer(unsigned width, unsigned height);
    TileLayer(TileLayer&& rhs) noexcept;

    void update(float dt);

    bool load(const tmx::Map& m, const tmx::TileLayer& t, const pixel::Tileset& tileset);

    const std::vector<Tile>& tiles() const;
    std::vector<Tile>& tiles();
    std::unordered_map<uint32_t, TileAnimation> animations();
    unsigned width() const;
    unsigned height() const;


    Tile& at(unsigned int x, unsigned int y)
    {
        return tiles_.at(x + y * height_);
    }


    const Tile& at(unsigned int x, unsigned int y) const
    {
        return tiles_.at(x + y * height_);
    }


private:
    std::unordered_map<uint32_t, TileAnimation> animations_;
    PropertyMap props_;
    std::vector<Tile> tiles_;
    unsigned int width_;
    unsigned int height_;
};

};

#endif //PIXEL_TILE_LAYER_H
