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

    void addTileset(const tmx::Tileset&);

    unsigned tileWidth() const { return _tile_width; };
    unsigned tileHeight() const { return _tile_height; };
    unsigned maxTiles() const { return _max_tiles; };
    unsigned atlasColumns() const { return _atlas_cols; };
    unsigned atlasRows() const { return _atlas_rows; };
    unsigned atlasLayers() const { return _atlas_layers; };
    uint16_t maxId() const { return _max_id; };
    uint16_t nextId();

    tuple<unsigned, unsigned, unsigned, unsigned> decodeId(uint16_t) const;

    uint16_t encodeId(uint8_t column, uint8_t row, uint8_t layer, uint8_t flags);

    void debugSave(const std::string& prefix);
};

}
#endif //PIXEL_TILESET_ATLAS_H
