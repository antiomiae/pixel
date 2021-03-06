#ifndef PIXEL_TILESET_H
#define PIXEL_TILESET_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include <tmxlite/Tileset.hpp>

namespace pixel
{

class Tileset
{

public:

    struct Tile
    {
        struct Animation
        {
            struct Frame
            {
                uint32_t tile_id{};

                float duration{};

                Frame(uint32_t tile_id, float duration)
                    : tile_id{tile_id},
                      duration{duration}
                { }
            };

            std::vector<Frame> frames{};

            void add_frame(uint32_t tile_id, float duration)
            {
                frames.emplace_back(tile_id, duration);
            }

        } animation;

        /**
         * Unique identifier of tile
        */
        uint32_t tile_id;
        std::string type;
    };

    using TileRegistry = std::unordered_map<uint32_t, Tile>;

    Tileset() = default;

    void add_tileset(const tmx::Tileset& tmx_tileset);
    bool tile_has_animation(uint32_t id) const;
    std::vector<Tile>& tiles();
    TileRegistry registry();
    Tile& tile(uint32_t id);

private:

    std::vector<Tile> tiles_;
    TileRegistry registry_;

};

};


#endif //PIXEL_TILESET_H
