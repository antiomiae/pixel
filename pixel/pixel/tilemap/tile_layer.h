#ifndef PIXEL_TILE_LAYER_H
#define PIXEL_TILE_LAYER_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <pixel/tilemap/tile_map.h>
#include <pixel/error.h>
#include "tileset.h"

namespace pixel
{

/* Forward declare to avoid circular include issues */
class TileMap;
class TileCoordinate;

using namespace std;

/**
 * Container of tile map data. Logically, a rectangular array of tile map tiles, regularly arranged and identically
 * sized objects that are drawn as a contiguous image, and which may carry additional information to be used in
 * collision detection or pathfinding.
 *
 * Maintains a collection of TileLayer::Tile.
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

        TileAnimation(uint32_t id, Tileset::Tile::Animation animation)
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

        void update(float dt);

        void update_frame();

        uint32_t current_tile_id();

    };

    using PropertyMap = std::unordered_map<uint32_t, Properties>;

    TileLayer(TileMap* parent);
    TileLayer(const TileLayer&) = default;

    bool load(const tmx::TileLayer& t);

    void update(float dt);

    const std::vector<Tile>& tiles() const;
    std::unordered_map<uint32_t, TileAnimation> animations();

    unsigned width() const;
    unsigned height() const;
    glm::vec2 parallax() const;

    Tile& at(uint x, uint y);

    TileMap& parent()
    {
        error_if(parent_ == nullptr, "parent not set!");
        return *parent_;
    }

    void visit_tiles(
        TileCoordinate p0,
        TileCoordinate p1,
        const function<bool(TileCoordinate, Tile&)>& cb
    )
    {
        for (auto y = min(p0.y, p1.y); y <= max(p0.y, p1.y); ++y) {
            for (auto x = min(p0.x, p1.x); x <= max(p0.x, p1.x); ++x) {
                auto should_continue = cb(TileCoordinate(x, y), at(x, y));

                if (!should_continue) {
                    return;
                }
            }
        }
    }



private:
    TileMap* parent_{nullptr};
    std::unordered_map<uint32_t, TileAnimation> animations_;
    PropertyMap props_;
    std::vector<Tile> tiles_;
    unsigned int width_;
    unsigned int height_;
    glm::vec2 parallax_;

};

};

#endif //PIXEL_TILE_LAYER_H
