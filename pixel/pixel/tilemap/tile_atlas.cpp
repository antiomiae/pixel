//
//

#include "tile_atlas.h"
#include <algorithm>
#include <boost/filesystem.hpp>

using namespace pixel;

using namespace std;

using boost::filesystem::create_directories;

using graphics::load_png;
using graphics::ImageData;
using std::to_string;

TileAtlas::TileAtlas(unsigned tile_width, unsigned tile_height, unsigned max_tiles)
  : _tile_width(tile_width),
    _tile_height(tile_height),
    _max_tiles(max_tiles),
    _max_id(0),
    _texture(GL_TEXTURE_2D_ARRAY)
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

    _atlas_layers = max(
      1u,
      min(MAX_ATLAS_LAYERS, _max_tiles / (_atlas_cols * _atlas_rows))
    );

    _texture.load(_tile_width * _atlas_cols, _tile_height * _atlas_rows, _atlas_layers);
}

void TileAtlas::addTileset(const tmx::Tileset& tileset)
{
    cout << "addTileset() begin" << endl;

    ImageData img = load_png(tileset.getImagePath());

    auto tiles = tileset.getTiles();

    auto first_id = tileset.getFirstGID();
    auto last_id = tileset.getLastGID();

    for (auto tile_idx = 0; tile_idx < tileset.getTileCount(); ++tile_idx) {
        auto atlas_id = nextId();
        auto tmx_id = first_id + tile_idx;

        _id_map[tmx_id] = atlas_id;

        unsigned col, row, layer, flags;
        tie(col, row, layer, flags) = decodeId(atlas_id);

        auto src_col = tile_idx % tileset.getColumnCount();
        auto src_row = tile_idx / tileset.getColumnCount();

        auto subregion = img.subregion(src_col * _tile_width, src_row * _tile_height, _tile_width, _tile_height);

        _texture.loadSubregion(col * _tile_width, row * _tile_height, _tile_width, _tile_height, layer, subregion.data);
    }

    cout << "addTileset() end" << endl;
}

std::tuple<unsigned, unsigned, unsigned, unsigned> TileAtlas::decodeId(uint16_t id) const
{
    return {
      (id & COLUMN_MASK) >> COLUMN_SHIFT,
      (id & ROW_MASK) >> ROW_SHIFT,
      (id & LAYER_MASK) >> LAYER_SHIFT,
      (id & FLAGS_MASK) >> FLAGS_SHIFT
    };
}

uint16_t TileAtlas::encodeId(uint8_t column, uint8_t row, uint8_t layer, uint8_t flags)
{
    return static_cast<uint16_t>(
      ((column << COLUMN_SHIFT) & COLUMN_MASK) |
      ((row << ROW_SHIFT) & ROW_MASK) |
      ((layer << LAYER_SHIFT) & LAYER_MASK) |
      ((flags << FLAGS_SHIFT) & FLAGS_MASK)
    );
}

uint16_t TileAtlas::nextId()
{
    auto n = _max_id + 1;

    unsigned col, row, layer, flags;
    tie(col, row, layer, flags) = decodeId(n);

    if (col >= _atlas_cols) {
        col = 0;
        ++row;
    }

    if (row >= _atlas_rows) {
        row = 0;
        ++layer;
    }

    if (layer >= _atlas_layers) {
        throw "Texture atlas out of space";
    }

    _max_id = encodeId(col, row, layer, flags);

    return _max_id;
}

void TileAtlas::debugSave(const std::string& prefix)
{
    create_directories(prefix);

    std::vector<uint8_t> pixels(_texture.storageSize());

    _texture.read(pixels.data());

    auto stride = _texture.width() * _texture.height() * 4;

    for (auto layer = 0u; layer < _atlas_layers; ++layer) {
        ImageData img(_texture.width(), _texture.height(), pixels.data() + layer * stride);
        img.save(prefix + to_string(layer) + ".png");
    }
}
