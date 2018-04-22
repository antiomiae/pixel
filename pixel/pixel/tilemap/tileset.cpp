

#include "tileset.h"

namespace pixel
{

void Tileset::add_tileset(const tmx::Tileset& tmx_tileset)
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

        registry_[tile.tile_id] = tile;
    };

}


std::vector<Tileset::Tile>& Tileset::tiles()
{
    return tiles_;
}


std::unordered_map<uint32_t, Tileset::Tile> Tileset::registry()
{
    return registry_;
}


bool Tileset::tile_has_animation(uint32_t id) const
{
    if (registry_.find(id) != cend(registry_)) {
        return !registry_.at(id).animation.frames.empty();
    }

    return false;
}


Tileset::Tile& Tileset::tile(uint32_t id)
{
    return registry_.at(id);
}

};
