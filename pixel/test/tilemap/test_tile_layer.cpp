#include "test/setup.h"

namespace
{

using namespace std;
using pixel::TileLayer;
using pixel::TileMap;

TEST_CASE("TileLayer")
{
    SECTION("manipulate tiles") {
        TileMap parent({30, 20}, {16, 16});

        TileLayer t(&parent);

        t.at(0, 0).tile_id = 0x101;

        auto tile = t.at(0, 0);

        REQUIRE(tile.tile_id == 0x101);
    }

    SECTION("load") {
        tmx::Map tmx_map;
        tmx_map.load("assets/traps_1.tmx");

        auto map_size = tmx_map.getTileCount();
        auto tile_size = tmx_map.getTileSize();

        TileMap tile_map{{map_size.x,  map_size.y},
                         {tile_size.x, tile_size.y}};

        for (auto& tmx_tileset : tmx_map.getTilesets()) {
            tile_map.atlas().add_tileset(tmx_tileset);
            tile_map.tileset().add_tileset(tmx_tileset);
        }

        const auto& tmx_layers = tmx_map.getLayers();

        const auto& reference_layer = dynamic_cast<tmx::TileLayer&>(**std::find_if(
            cbegin(tmx_layers),
            cend(tmx_layers),
            [](const unique_ptr<tmx::Layer>& layer) {
                return layer->getType() == tmx::Layer::Type::Tile;
            }
        ));


        TileLayer our_layer(&tile_map);

        our_layer.load(reference_layer);


        const auto& reference_tiles = reference_layer.getTiles();
        // Check that our map has any non-zero data. Sanity check.
        REQUIRE(any_of(cbegin(reference_tiles), cend(reference_tiles), [](auto t) { return t.ID > 0; }));
        /* Check that our layer contents are the same size */
        REQUIRE(our_layer.tiles().size() == reference_tiles.size());


        auto& our_tiles = our_layer.tiles();

        /* Counter to help ensure we exercise animation loading */
        int anim_count = 0;

        for (auto y = 0u; y < our_layer.height(); ++y) {
            for (auto x = 0u; x < our_layer.width(); ++x) {

                auto reference_tile = reference_tiles[x + (our_layer.height() - 1 - y) * our_layer.width()];
                auto layer_tile = our_layer.at(x, y);

                REQUIRE(reference_tile.ID == layer_tile.tile_id);

                if (tile_map.tileset().tile_has_animation(layer_tile.tile_id)) {
                    /* Expect to have loaded an animation */
                    REQUIRE_NOTHROW(
                        our_layer.animations().at(layer_tile.tile_id)
                    );

                    anim_count++;
                }
            }
        }

        /* Assert that we tested animations */
        REQUIRE(anim_count > 0);

    }
};

};
