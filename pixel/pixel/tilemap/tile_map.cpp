//
//

#include "tile_map.h"

using namespace pixel;

bool TileMap::load(const tmx::Map& map)
{
    auto [tw, th] = map.getTileSize();
    _atlas = make_unique<TileAtlas>(tw, th, 4096);

    for (auto& tileset : map.getTilesets())
    {
        _atlas->addTileset(tileset);
    }

    return true;
}

pixel::TileMap::TileMap()
{

}
