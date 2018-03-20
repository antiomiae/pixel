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

local function parse_layer(l)
end

function TileMap:__init(o)
    assert(o.tile_map, "requires pixel::TileMap argument tile_map")

    self.tile_map = o.tile_map
    self.tileset = self.tile_map:tileset()
    self.layers = {}

    self.layer_count = #self.tile_map:layers()
    for _, layer in ipairs(self.tile_map:layers()) do
        table.insert(self.layers, parse_layer(layer))
    end
end

function TileMap:get_tile_properties(x, y, layer_num)
    local tile_count = self.tile_map:tile_count()

    assert(x < tile_count.x and x >= 0)
    assert(y < tile_count.y and y >= 0)

    local layer = self.tile_map:layers()[layer_num]

end

return {
    TileMap = TileMap
}
