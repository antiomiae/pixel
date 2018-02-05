//
//

#ifndef PIXEL_TILESET_ATLAS_H
#define PIXEL_TILESET_ATLAS_H

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <tmxlite/Tileset.hpp>

namespace pixel
{

const unsigned MAX_ATLAS_WIDTH = 2048;
const unsigned MAX_ATLAS_HEIGHT = 2048;
const unsigned MAX_ATLAS_LAYERS = 16;
const unsigned MAX_ATLAS_COLUMNS = 16;
const unsigned MAX_ATLAS_ROWS = 16;

const unsigned COLUMN_MASK = 0x000F;
const unsigned ROW_MASK = 0x00F0;
const unsigned LAYER_MASK = 0x0F00;
const unsigned FLAGS_MASK = 0xF000;

/**
 * Combines one or more tilesets into a single texture array and maintains
 * a mapping between original tileset IDs and optimized
 */
class TileAtlas
{
private:
    std::unordered_map<uint32_t, uint16_t> _id_map;
    unsigned _tile_width, _tile_height, _max_tiles, _atlas_rows, _atlas_cols, _atlas_layers;
public:
    TileAtlas(unsigned tile_width, unsigned tile_height, unsigned max_tiles);
    void addTileset(const tmx::Tileset&);
};

}
#endif //PIXEL_TILESET_ATLAS_H
