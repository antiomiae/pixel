#include "test/setup.h"

namespace
{

using pixel::TileLayer;

TEST(TileLayer, ReferenceTile)
{
    TileLayer t{10, 10};

    t.at(0, 0).atlas_id = 0x101;

    auto tile = t.at(0, 0);

    ASSERT_TRUE(tile.atlas_id == 0x101);
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
    pixel::Tileset tileset;

    for (auto& tmx_tileset : tmx_map.getTilesets()) {
        atlas.add_tileset(tmx_tileset);
        tileset.add_tileset(tmx_tileset);
    }

    const auto& tmx_layers = tmx_map.getLayers();

    const auto& reference_layer = dynamic_cast<tmx::TileLayer&>(**std::find_if(
        cbegin(tmx_layers),
        cend(tmx_layers),
        [](const unique_ptr<tmx::Layer>& layer) {
            return layer->getType() == tmx::Layer::Type::Tile;
        }
    ));

    TileLayer our_layer{};
    our_layer.load(tmx_map, reference_layer, tileset, atlas);

    const auto& reference_tiles = reference_layer.getTiles();
    // Check that our map has any non-zero data. Sanity check.
    ASSERT_TRUE(any_of(cbegin(reference_tiles), cend(reference_tiles), [](auto t) { return t.ID > 0; }));
    /* Check that our layer contents are the same size */
    ASSERT_TRUE(our_layer.tiles().size() == reference_tiles.size());


    auto our_tiles = our_layer.tiles();

    /* Counter to help ensure we exercise animation loading */
    int anim_count = 0;

    for (auto i = 0u; i < our_layer.tiles().size(); ++i) {
        auto tmx_tile = reference_tiles[i];

        ASSERT_TRUE(
            our_tiles[i].atlas_id == atlas.atlas_id_from_tmx_id(tmx_tile.ID, tmx_tile.flipFlags)
        );

        if (tileset.tile_has_animation(tmx_tile.ID)) {
            /* Expect to have loaded an animation */
            EXPECT_NO_THROW(
                our_layer.animations().at(tmx_tile.ID)
            );
            anim_count++;
        }
    }

    /* Assert that we tested animations */
    ASSERT_TRUE(anim_count > 0);
};

};
