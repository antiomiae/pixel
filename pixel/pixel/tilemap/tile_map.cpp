#include <pixel/error.h>
#include "tile_map.h"
#include "tile_layer.h"

using namespace std;
using namespace pixel;


TileMap::TileMap(const glm::uvec2& map_size, const glm::uvec2& tile_size)
    : tile_count_{map_size},
      tile_size_{tile_size}
{
    atlas_ = make_unique<TileAtlas>(tile_size_.x, tile_size_.y, 4096);
}


unique_ptr<TileMap> TileMap::from_path(const std::string& tmx_path)
{
    tmx::Map m;

    error_unless(m.load(tmx_path), "Unable to load tmx map file at path " + tmx_path);

    auto tm = make_unique<TileMap>(
        glm::uvec2 {m.getTileCount().x, m.getTileCount().y},
        glm::uvec2 {m.getTileSize().x, m.getTileSize().y}
    );

    error_unless(tm->load(m), "Unable to load map into TileMap");

    return tm;
}


bool TileMap::load(const tmx::Map& map)
{

    auto [tw, th] = map.getTileSize();

    error_if(tw != tile_size_.x || th != tile_size_.y,
                      "tmx::Map tile size does not match this TileMap's tile size");

    auto [x, y] = map.getTileCount();

    error_if(x != tile_count_.x || y != tile_count_.y,
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


void TileMap::update(float dt)
{
    for (auto& layer : layers_) {
        layer.update(dt);
    }
}


Tileset& TileMap::tileset()
{
    return tileset_;
}
