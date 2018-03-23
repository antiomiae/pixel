#include <pixel/tilemap/tileset.h>
#include "lua_binding.h"

namespace pixel {
void bind_tileset(sol::state& lua, sol::table& binding, const string& type_name)
{
    auto tileset = binding.new_usertype<Tileset>(
        type_name,
        //"new", sol::constructors<Tileset()>(),
        "tile", &Tileset::tile
    );

    auto tile = tileset.new_usertype<Tileset::Tile>(
        "Tile",
        "animation", &Tileset::Tile::animation,
        "tile_id", &Tileset::Tile::tile_id,
        "type", &Tileset::Tile::type
    );
}
}
