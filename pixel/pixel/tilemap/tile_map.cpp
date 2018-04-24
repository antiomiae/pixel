#include <pixel/error.h>
#include "tile_map.h"

using namespace pixel;

bool TileMap::load(const tmx::Map& map)
{
    auto [tw, th] = map.getTileSize();
    atlas_ = make_unique<TileAtlas>(tw, th, 4096);

    tile_size_ = {tw, th};
    {
        auto [x, y] = map.getTileCount();
        tile_count_ = {x, y};
    }

    for (auto& tileset : map.getTilesets()) {
        atlas_->add_tileset(tileset);
        tileset_.add_tileset(tileset);
    }

    for (auto const& tmx_layer : map.getLayers()) {
        if (const auto layer = dynamic_cast<tmx::TileLayer*>(tmx_layer.get())) {
            layers_.emplace_back();
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
