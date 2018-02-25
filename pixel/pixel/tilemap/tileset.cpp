

#include "tileset.h"


void pixel::Tileset::add_tileset(const tmx::Tileset& tmx_tileset)
{
    auto first_id = tmx_tileset.getFirstGID();

    for (auto& tmx_tile : tmx_tileset.getTiles()) {
        tiles_.emplace_back();
        auto& tile = tiles_.back();

        tile.type = tmx_tile.type;
        tile.tile_id = tmx_tile.ID + first_id;

        auto has_animation = !tmx_tile.animation.frames.empty();

        if (has_animation) {
            for (auto& frame : tmx_tile.animation.frames) {
                tile.animation.add_frame(frame.tileID + first_id, frame.duration);
            }
        }

        id_map_[tile.tile_id] = tile;
    };

}


const std::vector<pixel::Tileset::Tile> pixel::Tileset::tiles() const
{
    return tiles_;
}


const std::unordered_map<uint32_t, pixel::Tileset::Tile> pixel::Tileset::id_map() const
{
    return id_map_;
}
