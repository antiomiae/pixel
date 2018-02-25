#include "test/setup.h"

namespace
{

using pixel::TileLayer;

TEST(TileLayer, ReferenceTile)
{
    TileLayer t{10, 10};

    t.at(0, 0).tile_id = 0x101;

    auto tile = t.at(0, 0);

    ASSERT_TRUE(tile.tile_id == 0x101);
};

TEST(TileLayer, DefaultConstructible)
{
    TileLayer t{};
};

TEST(TileLayer, Load)
{
    string map_file = "assets/traps_1.tmx";

    tmx::Map tmx_map;
    tmx_map.load(map_file);

    pixel::TileAtlas atlas{tmx_map.getTileSize().x, tmx_map.getTileSize().y, 100};

    for (auto& tileset : tmx_map.getTilesets()) {
        atlas.add_tileset(tileset);
    }

    const auto& tmx_layers = tmx_map.getLayers();

    const auto& tmx_tile_layer = dynamic_cast<tmx::TileLayer&>(**std::find_if(
        cbegin(tmx_layers),
        cend(tmx_layers),
        [](const unique_ptr<tmx::Layer>& layer) {
            return layer->getType() == tmx::Layer::Type::Tile;
        }
    ));

    TileLayer layer{};
    layer.load(tmx_map, tmx_tile_layer, atlas);

    const auto& tmx_tiles = tmx_tile_layer.getTiles();
    // Check that our map has any non-zero data. Sanity check.
    ASSERT_TRUE(any_of(cbegin(tmx_tiles), cend(tmx_tiles), [](auto t) { return t.ID > 0; }));

    ASSERT_TRUE(layer.tiles().size() == tmx_tiles.size());

    for (auto i = 0u; i < layer.tiles().size(); ++i) {
        auto tmx_tile = tmx_tiles[i];

        ASSERT_TRUE(
            layer.tiles()[i].tile_id == atlas.atlas_id_from_tmx_id(tmx_tile.ID, tmx_tile.flipFlags)
        );
    }
};

};
