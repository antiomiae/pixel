#include <pixel/error.h>
#include "tile_map.h"

using namespace pixel;

bool TileMap::load(const tmx::Map& map)
{

    auto [tw, th] = map.getTileSize();

    argument_error_if(tw != tile_size_.x || th != tile_size_.y,
                      "tmx::Map tile size does not match this TileMap's tile size");

    auto [x, y] = map.getTileCount();

    argument_error_if(x != tile_count_.x || y != tile_count_.y,
                      "tmx::Map map dimensions do not match this TileMap's dimensions");


    for (auto& tileset : map.getTilesets()) {
        atlas_->add_tileset(tileset);
        tileset_.add_tileset(tileset);
    }

    for (auto const& tmx_layer : map.getLayers()) {
        if (const auto layer = dynamic_cast<tmx::TileLayer*>(tmx_layer.get())) {
            layers_.emplace_back(this);
            layers_.back().load(*layer);
        }
    }

    return true;
}


TileAtlas& TileMap::atlas()
{
    return *atlas_;
}


vector<TileLayer>& TileMap::layers()
{
    return layers_;
}


glm::uvec2 TileMap::tile_count() const
{
    return tile_count_;
}


glm::uvec2 TileMap::tile_size() const
{
    return tile_size_;
}


void TileMap::set_tile_count(const glm::uvec2& v)
{
    tile_count_ = v;
}


void TileMap::set_tile_size(const glm::uvec2& v)
{
    tile_size_ = v;
}


void TileMap::update(float dt)
{
    for (auto& layer : layers_) {
        layer.update(dt);
    }
}


bool TileMap::load(const std::string& tmx_path)
{
    tmx::Map m;
    argument_error_unless(m.load(tmx_path), "Unable to load tmx map file at path " + tmx_path);
    return load(m);
}

Tileset& TileMap::tileset()
{
    return tileset_;
}

TileMap::TileMap(const glm::uvec2& map_size, const glm::uvec2& tile_size)
: tile_count_{map_size},
  tile_size_{tile_size}
{
    atlas_ = make_unique<TileAtlas>(tile_size_.x, tile_size_.y, 4096);
}
