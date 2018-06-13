#include <iostream>
#include "tile_atlas.h"
#include <boost/filesystem.hpp>

using namespace pixel;

using namespace std;

using boost::filesystem::create_directories;
using boost::filesystem::path;

using graphics::load_png;
using graphics::ImageData;


TileAtlas::TileAtlas(unsigned tile_width, unsigned tile_height, unsigned max_tiles)
    : tile_width_(tile_width),
      tile_height_(tile_height),
      max_tiles_(max_tiles),
      max_id_(0),
      texture_(GL_TEXTURE_2D_ARRAY)
{
    if (tile_width_ * MAX_ATLAS_COLUMNS <= MAX_ATLAS_WIDTH) {
        atlas_cols_ = MAX_ATLAS_COLUMNS;
    } else {
        atlas_cols_ = MAX_ATLAS_WIDTH / tile_width_;
    }

    if (tile_height_ * MAX_ATLAS_ROWS <= MAX_ATLAS_HEIGHT) {
        atlas_rows_ = MAX_ATLAS_ROWS;
    } else {
        atlas_rows_ = MAX_ATLAS_HEIGHT / tile_height_;
    }

    atlas_layers_ = max(
        1u,
        min(MAX_ATLAS_LAYERS, max_tiles_ / (atlas_cols_ * atlas_rows_))
    );

    texture_.load(tile_width_ * atlas_cols_, tile_height_ * atlas_rows_, atlas_layers_);
}


void TileAtlas::add_tileset(const tmx::Tileset& tileset)
{
    ImageData img = load_png(tileset.getImagePath());

    auto first_id = tileset.getFirstGID();
    auto last_id = tileset.getLastGID();

    for (auto& tile : tileset.getTiles()) {
        auto atlas_id = next_atlas_id();
        id_registry_[tile.ID + first_id] = atlas_id;

        unsigned col, row, layer, flags;
        tie(col, row, layer, flags) = decode_atlas_id(atlas_id);

        auto image_size = tile.imageSize;

        if (image_size.x != tile_width_ || image_size.y != tile_height_) {
            cout << "Warning: The tile size of this atlas is configured with does not match the tileset it is loading"
                 << endl;
        }

        auto image_position = tile.imagePosition;
        auto type = tile.type;

        auto subregion = img.subregion(image_position.x, image_position.y, tile_width_, tile_height_);
        texture_.load_subregion(col * tile_width_, row * tile_height_, tile_width_, tile_height_, layer, subregion.data);
    };
}


tuple<unsigned, unsigned, unsigned, unsigned> TileAtlas::decode_atlas_id(uint16_t id) const
{
    return {
        (id & COLUMN_MASK) >> COLUMN_SHIFT,
        (id & ROW_MASK) >> ROW_SHIFT,
        (id & LAYER_MASK) >> LAYER_SHIFT,
        (id & FLAGS_MASK) >> FLAGS_SHIFT
    };
}


uint16_t TileAtlas::encode_atlas_id(uint8_t column, uint8_t row, uint8_t layer, uint8_t flags) const
{
    return static_cast<uint16_t>(
        ((column << COLUMN_SHIFT) & COLUMN_MASK) |
        ((row << ROW_SHIFT) & ROW_MASK) |
        ((layer << LAYER_SHIFT) & LAYER_MASK) |
        ((flags << FLAGS_SHIFT) & FLAGS_MASK)
    );
}


uint16_t TileAtlas::next_atlas_id()
{
    unsigned col, row, layer, flags;
    tie(col, row, layer, flags) = decode_atlas_id(max_id_ + 1);

    if (col >= atlas_cols_) {
        col = 0;
        ++row;
    }

    if (row >= atlas_rows_) {
        row = 0;
        ++layer;
    }

    if (layer >= atlas_layers_) {
        throw "Texture atlas out of space";
    }

    max_id_ = encode_atlas_id(col, row, layer, flags);

    return max_id_;
}


void TileAtlas::debug_save(const string& prefix, const string& dir)
{
    if (dir != ".") {
        create_directories(dir);
    }

    vector<uint8_t> pixels(texture_.storage_size());

    /* Copy pixel data from texture to our buffer */
    texture_.read(pixels.data());

    auto stride = texture_.width() * texture_.height() * 4;

    for (auto layer = 0u; layer < atlas_layers_; ++layer) {
        ImageData img{texture_.width(), texture_.height(), pixels.data() + layer * stride};

        auto filename{prefix + to_string(layer) + ".png"};

        img.save((path(dir) / filename).string());
    }
}


uint16_t TileAtlas::max_atlas_id() const
{ return max_id_; }


unsigned TileAtlas::tile_width() const
{ return tile_width_; }


unsigned TileAtlas::tile_height() const
{ return tile_height_; }


unsigned TileAtlas::max_tiles() const
{ return max_tiles_; }


unsigned TileAtlas::atlas_columns() const
{ return atlas_cols_; }


unsigned TileAtlas::atlas_rows() const
{ return atlas_rows_; }


unsigned TileAtlas::atlas_layers() const
{ return atlas_layers_; }


const uint16_t TileAtlas::atlas_id_from_tmx_id(uint32_t tmx_id) const
{
    return (tmx_id == 0 ? 0 : id_registry_.at(tmx_id));
}

const uint16_t TileAtlas::atlas_id_from_tmx_id(uint32_t tmx_id, uint8_t flags) const
{
    if (tmx_id != 0) {
        auto parts = decode_atlas_id(atlas_id_from_tmx_id(tmx_id));
        return encode_atlas_id(get<0>(parts), get<1>(parts), get<2>(parts), flags);
    }

    return 0;
}

void TileAtlas::activate_texture(unsigned texture_unit)
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    texture_.bind();
}



