//
//

#include "tileset_atlas.h"

using namespace pixel;

TileAtlas::TileAtlas(unsigned tile_width, unsigned tile_height, unsigned max_tiles)
: _tile_width(tile_width),
  _tile_height(tile_height),
  _max_tiles(max_tiles)
{
    if (_tile_width * MAX_ATLAS_COLUMNS <= MAX_ATLAS_WIDTH) {
        _atlas_cols = MAX_ATLAS_COLUMNS;
    } else {
        _atlas_cols = MAX_ATLAS_WIDTH / _tile_width;
    }

    if (_tile_height * MAX_ATLAS_ROWS <= MAX_ATLAS_HEIGHT) {
        _atlas_rows = MAX_ATLAS_ROWS;
    } else {
        _atlas_rows = MAX_ATLAS_HEIGHT / _tile_height;
    }

    _atlas_layers = (unsigned) std::ceil((float) _max_tiles / (_atlas_cols * _atlas_rows));
}

void TileAtlas::addTileset(const tmx::Tileset& tileset)
{

}
