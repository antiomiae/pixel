-- Tile map with collision
local TileMap = {}

function TileMap:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    if o.__init then
        o:__init(o)
    end

    return o
end

local function parse_layer(layer, tileset)
    local w = layer:width()
    local h = layer:height()
    local tiles = {}

    for row = 0, h-1 do
        local _row = h - row - 1
        tiles[_row] = {}

        for col = 0, w-1 do

            local tile = layer:at(col, row)

            if tile.tile_id ~= 0 then
                local tileset_tile = tileset:tile(tile.tile_id)
                local name = tileset_tile.type

                tiles[_row][col] = { tile = tile, name = name }
            else
                tiles[_row][col] = { tile = tile }
            end
        end
    end

    return tiles
end

function TileMap:__init(o)
    assert(o.tile_map, "requires pixel::TileMap argument tile_map")

    self.tile_map = o.tile_map
    self.tileset = self.tile_map:tileset()
    self.layers = {}

    self.tile_size = self.tile_map:tile_size()
    self.tile_count = self.tile_map:tile_count()

    self.w = self.tile_count.x
    self.h = self.tile_count.y

    self.tw = self.tile_size.x
    self.th = self.tile_size.y

    self.layer_count = #self.tile_map:layers()

    for _, layer in ipairs(self.tile_map:layers()) do
        table.insert(self.layers, parse_layer(layer, self.tileset))
    end
end

function TileMap:get_tile_properties(x, y, layer_num)
    assert(x < self.w and x >= 0)
    assert(y < self.h and y >= 0)
    assert(layer_num <= #self.layers and layer_num >= 1)

    local layer = self.layers[layer_num]
    local tile = layer[y][x]

    return tile
end

function TileMap:tile_at_coord(coord, layer_num)
    local col = coord.x // self.tw
    local row = coord.y // self.th

    assert(col < self.w and col >= 0)
    assert(row < self.h and row >= 0)

    return self:get_tile_properties(col, row, layer_num)
end

function TileMap:get_overlapping_tiles(area, layer_num)
    -- check area overlaps tilemap
    if not pixel.collision.rects_overlap(area, {x = 0, y = 0, w = self.w * self.tw, h = self.h * self.th}) then
        return nil
    end

    local start_x = math.max(0, area.x) // self.tw
    local start_y = math.max(0, area.y) // self.th
    local stop_x = math.min(self.tw * self.w, area.x + area.w) // self.tw
    local stop_y = math.min(self.th * self.h, area.y + area.h) // self.th

    local out = {}

    for x = start_x, stop_x do
        for y = start_y, stop_y do
            out[{x = x, y = y}] = self:tile_at_coord({x = x, y = y}, layer_num)
        end
    end

    return out
end

function TileMap:has_coord(coord)
    return (coord.x >= 0) and (coord.x < self.w * self.tw) and
            (coord.y >= 0) and (coord.y < self.h * self.th)
end

return TileMap
