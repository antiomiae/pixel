#include "test/setup.h"

namespace
{

using namespace pixel;


TEST(Tileset, DefaultConstructible)
{
    Tileset t{};
};

TEST(Tileset, add_tileset)
{
    string map_file = "assets/traps_1.tmx";

    tmx::Map tmx_map;
    tmx_map.load(map_file);

    Tileset tileset;

    for (auto& tmx_tileset : tmx_map.getTilesets()) {
        tileset.add_tileset(tmx_tileset);

        const auto& tiles = tileset.tiles();
        const auto& tile_registry = tileset.registry();
        auto first_id = tmx_tileset.getFirstGID();

        for (auto& tmx_tile : tmx_tileset.getTiles()) {
            if (tile_registry.find(tmx_tile.ID + first_id) != end(tile_registry)) {
                const auto& stored_tile = tile_registry.at(tmx_tile.ID + first_id);
                EXPECT_TRUE(stored_tile.animation.frames.size() == tmx_tile.animation.frames.size());
                EXPECT_TRUE(
                    std::any_of(
                        cbegin(tiles), cend(tiles),
                        [&](const Tileset::Tile& t) { return t.tile_id == stored_tile.tile_id; }
                    )
                );
            } else {
                ADD_FAILURE() << "id not found in registry";
            }

        }
    }
};

};
