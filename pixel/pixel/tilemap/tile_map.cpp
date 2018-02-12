//
//

#include "tile_map.h"

using namespace pixel;


bool TileMap::load(const tmx::Map& map)
{
    auto[tw, th] = map.getTileSize();
    _atlas = make_unique<TileAtlas>(tw, th, 4096);

    _tileSize = {tw, th};
    {
        auto [x, y] = map.getTileCount();
        _tileCount = {x, y};
    }

    for (auto& tileset : map.getTilesets()) {
        _atlas->addTileset(tileset);
    }

    for (auto const& tmx_layer : map.getLayers()) {
        if (const auto layer = dynamic_cast<tmx::TileLayer*>(tmx_layer.get())) {
            _layers.emplace_back(map, *layer, *_atlas);
        }
    }

    return true;
}


pixel::TileMap::TileMap()
{
}


TileAtlas& TileMap::atlas() const
{
    return *_atlas;
}


const vector<TileLayer>& TileMap::layers() const
{
    return _layers;
}


glm::vec2 TileMap::tileCount() const
{
    return _tileCount;
}


glm::vec2 TileMap::tileSize() const
{
    return _tileSize;
}
