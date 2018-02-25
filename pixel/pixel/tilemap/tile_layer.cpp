#include <pixel/pixel.h>

using namespace pixel;
using namespace std;


TileLayer::TileLayer(unsigned width, unsigned height)
    : width_(width),
      height_(height),
      tiles_(width * height)
{
    init();
}


void TileLayer::init()
{
    texture_ = make_unique<Texture>(GL_TEXTURE_2D, GL_RED_INTEGER, GL_R16UI, GL_UNSIGNED_SHORT);
    (*texture_).load(width_, height_, nullptr);
}


TileLayer::TileLayer(TileLayer&& rhs) noexcept
    : texture_(std::move(rhs.texture_)),
      width_(rhs.width_),
      height_(rhs.height_),
      tiles_(move(rhs.tiles_))
{
}

bool TileLayer::load(const tmx::Map& m, const tmx::TileLayer& t, const pixel::Tileset& tileset, const pixel::TileAtlas& atlas)
{

    auto tile_count = m.getTileCount();

    if (width_ > 0 || height_ > 0) {
        argument_error_if(
            tile_count.x != width_ || tile_count.y != height_,
            "TileLayer was initialized with different map size than supplied tmx::Map"
        );

    } else {
        width_ = tile_count.x;
        height_ = tile_count.y;
        tiles_.resize(width_ * height_);
    }

    if (!texture_) {
        init();
    }

    auto tmx_tiles = t.getTiles();
    error_if(tmx_tiles.size() != tiles_.size(), "tmx::TileLayer t contains different number of tiles than expected");

    transform(
        begin(tmx_tiles),
        end(tmx_tiles),
        begin(tiles_),
        [&](auto tmx_tile) {
            return Tile{
                tmx_tile.ID,
                tmx_tile.flipFlags,
                atlas.atlas_id_from_tmx_id(tmx_tile.ID, tmx_tile.flipFlags)
            };
        }
    );

    for (auto i = 0u; i < tiles_.size(); ++i) {
        auto& tile = tiles_[i];
        if (tileset.tile_has_animation(tile.tile_id)) {
            animations_[tile.tile_id].tile_id = tile.tile_id;
            animations_[tile.tile_id].animation_definition = tileset.tile(tile.tile_id).animation;
            animations_[tile.tile_id].tiles.push_back(i);
        }
    }

    this->update_texture();

    return true;
}


Texture& TileLayer::texture() const
{
    return *texture_;
}


const vector<TileLayer::Tile>& TileLayer::tiles() const
{
    return tiles_;
}


vector<TileLayer::Tile>& TileLayer::tiles()
{
    return tiles_;
}


void TileLayer::update_texture()
{
    vector<uint16_t> tex_data(tiles_.size());
    std::transform(
        begin(tiles_),
        end(tiles_),
        begin(tex_data),
        [](auto& tile) { return tile.atlas_id; }
    );
    (*texture_).loadSubregion(0, 0, width_, height_, tex_data.data());

}


unordered_map<uint32_t, TileLayer::TileAnimation> TileLayer::animations()
{
    return animations_;
}


