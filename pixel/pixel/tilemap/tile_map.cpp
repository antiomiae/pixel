//
//

#include "tile_map.h"

using namespace pixel;


bool TileMap::load(const tmx::Map& map)
{
    auto[tw, th] = map.getTileSize();
    atlas_ = make_unique<TileAtlas>(tw, th, 4096);

    tile_size_ = {tw, th};
    {
        auto [x, y] = map.getTileCount();
        tile_count_ = {x, y};
    }

    for (auto& tileset : map.getTilesets()) {
        atlas_->addTileset(tileset);
    }

    for (auto const& tmx_layer : map.getLayers()) {
        if (const auto layer = dynamic_cast<tmx::TileLayer*>(tmx_layer.get())) {
            layers_.emplace_back(map, *layer, *atlas_);
        }
    }

    return true;
}


pixel::TileMap::TileMap()
{
}


TileAtlas& TileMap::atlas() const
{
    return *atlas_;
}


const vector<TileLayer>& TileMap::layers() const
{
    return layers_;
}


glm::vec2 TileMap::tile_count() const
{
    return tile_count_;
}


glm::vec2 TileMap::tile_size() const
{
    return tile_size_;
}
