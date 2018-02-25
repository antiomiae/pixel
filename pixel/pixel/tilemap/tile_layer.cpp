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


TileLayer::TileLayer(const tmx::Map& m, const tmx::TileLayer& t, const TileAtlas& atlas)
    : TileLayer(m.getTileCount().x, m.getTileCount().y)
{
    load(m, t, atlas);
}


bool TileLayer::load(const tmx::Map& m, const tmx::TileLayer& t, const pixel::TileAtlas& atlas)
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
            return Tile{atlas.atlas_id_from_tmx_id(tmx_tile.ID, tmx_tile.flipFlags)};
        }
    );

    (*texture_).loadSubregion(0, 0, width_, height_, tiles_.data());

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
