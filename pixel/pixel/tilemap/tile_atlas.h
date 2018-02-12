//
//

#ifndef PIXEL_TILESET_ATLAS_H
#define PIXEL_TILESET_ATLAS_H

#include "../graphics/graphics.h"
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <tmxlite/Tileset.hpp>

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
private:
    std::unordered_map<uint32_t, uint16_t> _id_map;
    unsigned _tile_width, _tile_height, _max_tiles, _atlas_rows, _atlas_cols, _atlas_layers;
    uint16_t _max_id;
    Texture _texture;
public:
    TileAtlas(unsigned tile_width, unsigned tile_height, unsigned max_tiles);

    /* Atlas state */
    void addTileset(const tmx::Tileset&);
    uint16_t genNextId();

    /* Helpers */
    const uint16_t atlasId(uint32_t tmx_id) const;
    uint16_t encodeId(uint8_t column, uint8_t row, uint8_t layer, uint8_t flags) const;
    tuple<unsigned, unsigned, unsigned, unsigned> decodeId(uint16_t) const;

    /* Debugging */
    void debugSave(const std::string& prefix, const std::string& dir = "debug");

    /* Getters */
    uint16_t maxId() const;
    unsigned tileWidth() const;
    unsigned tileHeight() const;
    unsigned maxTiles() const;
    unsigned atlasColumns() const;
    unsigned atlasRows() const;
    unsigned atlasLayers() const;

};

}
#endif //PIXEL_TILESET_ATLAS_H
