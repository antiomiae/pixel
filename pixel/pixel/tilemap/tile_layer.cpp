#include <pixel/pixel.h>

using namespace pixel;
using namespace std;


TileLayer::TileLayer(unsigned width, unsigned height)
    : width_(width),
      height_(height),
      tiles_(width * height)
{
}

TileLayer::TileLayer(TileLayer&& rhs) noexcept
    : width_(rhs.width_),
      height_(rhs.height_),
      tiles_(move(rhs.tiles_))
{
}

bool TileLayer::load(const tmx::Map& m, const tmx::TileLayer& t, const pixel::Tileset& tileset)
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

    auto tmx_tiles = t.getTiles();
    error_if(tmx_tiles.size() != tiles_.size(), "tmx::TileLayer t contains different number of tiles than expected");

    transform(
        begin(tmx_tiles),
        end(tmx_tiles),
        begin(tiles_),
        [&](auto tmx_tile) {
            return Tile{
                tmx_tile.ID,
                tmx_tile.flipFlags
            };
        }
    );

    for (auto i = 0u; i < tiles_.size(); ++i) {
        auto& tile = tiles_[i];
        if (tileset.tile_has_animation(tile.tile_id)) {
            animations_[tile.tile_id].base_tile_id = tile.tile_id;
            animations_[tile.tile_id].animation_definition = tileset.tile(tile.tile_id).animation;
            animations_[tile.tile_id].tiles.push_back(i);
        }
    }

    return true;
}


const vector<TileLayer::Tile>& TileLayer::tiles() const
{
    return tiles_;
}


vector<TileLayer::Tile>& TileLayer::tiles()
{
    return tiles_;
}


unordered_map<uint32_t, TileLayer::TileAnimation> TileLayer::animations()
{
    return animations_;
}


unsigned TileLayer::width() const
{
    return width_;
}


unsigned TileLayer::height() const
{
    return height_;
}


void TileLayer::update(float dt)
{
    for (auto& [id, anim] : animations_) {
        anim.update(dt);

        if (!anim.tiles.empty()) {
            for (auto tile_idx : anim.tiles) {
                tiles_[tile_idx].tile_id = anim.tile_id();
            }
        }
    }
}


