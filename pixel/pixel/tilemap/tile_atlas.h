#ifndef PIXEL_TILESET_ATLAS_H
#define PIXEL_TILESET_ATLAS_H

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <tmxlite/Tileset.hpp>
#include <pixel/graphics/graphics.h>

namespace pixel
{

using namespace std;
using graphics::Texture;

const unsigned MAX_ATLAS_WIDTH = 2048;
const unsigned MAX_ATLAS_HEIGHT = 2048;
const unsigned MAX_ATLAS_LAYERS = 256;
const unsigned MAX_ATLAS_COLUMNS = 16;
const unsigned MAX_ATLAS_ROWS = 16;

const unsigned COLUMN_MASK = 0x000F;
const unsigned ROW_MASK = 0x00F0;
const unsigned LAYER_MASK = 0x0F00;
const unsigned FLAGS_MASK = 0xF000;

const unsigned COLUMN_SHIFT = 0;
const unsigned ROW_SHIFT = 4;
const unsigned LAYER_SHIFT = 8;
const unsigned FLAGS_SHIFT = 12;

/**
 * Combines one or more tilesets into a single texture array and maintains
 * a mapping between original tileset IDs and optimized IDs
 */
class TileAtlas
{

public:

    TileAtlas(unsigned tile_width, unsigned tile_height, unsigned max_tiles);

    /* Atlas state */
    void add_tileset(const tmx::Tileset&);
    uint16_t next_atlas_id();

    /* Helpers */
    const uint16_t atlas_id_from_tmx_id(uint32_t tmx_id) const;
    const uint16_t atlas_id_from_tmx_id(uint32_t tmx_id, uint8_t flags) const;
    uint16_t encode_atlas_id(uint8_t column, uint8_t row, uint8_t layer, uint8_t flags) const;
    tuple<unsigned, unsigned, unsigned, unsigned> decode_atlas_id(uint16_t) const;

    /* Debugging */
    void debug_save(const std::string& prefix, const std::string& dir = "debug");

    /* Getters */
    uint16_t max_atlas_id() const;
    unsigned tile_width() const;
    unsigned tile_height() const;
    unsigned max_tiles() const;
    unsigned atlas_columns() const;
    unsigned atlas_rows() const;
    unsigned atlas_layers() const;

    void activate_texture(unsigned texture_unit);

private:

    std::unordered_map<uint32_t, uint16_t> id_registry_;
    unsigned tile_width_, tile_height_, max_tiles_, atlas_rows_, atlas_cols_, atlas_layers_;
    uint16_t max_id_;
    Texture texture_;

};

}
#endif //PIXEL_TILESET_ATLAS_H
