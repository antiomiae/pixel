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

    TileLayer layer{};

    const auto& tmx_layers = tmx_map.getLayers();

    const auto& tmx_tile_layer = *std::find_if(
        begin(tmx_layers),
        end(tmx_layers),
        [](const unique_ptr<tmx::Layer>& layer) {
            return layer->getType() == tmx::Layer::Type::Tile;
        }
    );

    layer.load(tmx_map, dynamic_cast<tmx::TileLayer&>(*tmx_tile_layer), atlas);
};

};
