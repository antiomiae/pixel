-- Tile map with collision
local inspect = require 'inspect'
local TileMap = {}

function TileMap:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    if o.__init then
        o:__init(o)
    end
end

function TileMap:__init(o)
    assert(o.tile_map, "requires pixel::TileMap argument tile_map")

    self.tile_map = o.tile_map
    self.tileset = self.tile_map:tileset()

    self.layer_count = #self.tile_map:layers()
    assert(self.layer_count)

    print(inspect(self))
end

function TileMap:get_tile_properties(x, y)
    assert(x < self.tile_map:tile_count().x)
    assert(x < self.tile_map:tile_count().y)
end

return {
    TileMap = TileMap
}
