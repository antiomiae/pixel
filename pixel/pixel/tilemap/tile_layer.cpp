#include <pixel/pixel.h>

using namespace pixel;
using namespace std;


TileLayer::TileLayer(unsigned width, unsigned height)
    : _width(width),
      _height(height),
      _tiles(width * height)
{
    init();
}


void TileLayer::init()
{
    texture_ = make_unique<Texture>(GL_TEXTURE_2D, GL_RED_INTEGER, GL_R16UI, GL_UNSIGNED_SHORT);
    (*texture_).load(_width, _height, nullptr);
}


TileLayer::TileLayer(TileLayer&& rhs) noexcept
    : texture_(std::move(rhs.texture_)),
      _width(rhs._width),
      _height(rhs._height),
      _tiles(move(rhs._tiles))
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

    if (_width > 0 || _height > 0) {
        argument_error_if(
            tile_count.x != _width || tile_count.y != _height,
            "TileLayer was initialized with different map size than supplied tmx::Map"
        );

    } else {
        _width = tile_count.x;
        _height = tile_count.y;
        _tiles.resize(_width * _height);
    }

    if (!texture_) {
        init();
    }

    auto tmx_tiles = t.getTiles();
    error_if(tmx_tiles.size() != _tiles.size(), "tmx::TileLayer t contains different number of tiles than expected");

    transform(
        begin(tmx_tiles),
        end(tmx_tiles),
        begin(_tiles),
        [&](auto tmx_tile) {
            return Tile{atlas.atlas_id_from_tmx_id(tmx_tile.ID, tmx_tile.flipFlags)};
        }
    );

    (*texture_).loadSubregion(0, 0, _width, _height, _tiles.data());

    return true;
}


Texture& TileLayer::texture() const
{
    return *texture_;
}
