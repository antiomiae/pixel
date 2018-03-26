pixel.inspect = require('inspect')
pixel.TileMapWrapper = require('tilemap')

require('animation')

-- pixel.load_map
function pixel.load_map(s)
    local tm = pixel.TileMap.new()
    tm:load(s)
    return tm
end

-- pixel.Level
pixel.Level = {}

function pixel.Level:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    o:__init()

    return o
end

function pixel.Level:__init()
    self.map_renderer = pixel.TileMapRenderer.new()
    self.sprite_renderer = pixel.SpriteRenderer.new()
    self.sprite_batch = pixel.SpriteBatch.new()
    self.maps = {}
    self.map_wrappers = {}
    self.animations = {}
    self.actors = {}

    local tsize = pixel.glm.uvec3.new(512, 512, 10)
    self.atlas = pixel.TextureAtlas:new(tsize)
    self.atlas_texture = nil

    self.camera = pixel.Camera:new()
    self.camera:set_window_size(320, 240)
end

function pixel.Level:load_sprites(sprites)
    self.atlas:start_batch()
    for path, name in pairs(sprites) do
        self.atlas:add_image(path, name)
    end
    self.atlas:stop_batch()
    self.atlas_texture = self.atlas:as_texture()
end

function pixel.Level:add_actor(a)
    table.insert(self.actors, a)
end

function pixel.Level:add_map(m)
    table.insert(self.maps, m)
    table.insert(self.map_wrappers, pixel.TileMapWrapper:new({ tile_map = m }))
end

function pixel.Level:add_animation(a)
    if type(a) == "string" then
        local anims = pixel.load_animations(a, self.atlas)

        for name, animation in pairs(anims) do
            self.animations[name] = animation
        end
    else
        self.animations[a.name] = a
    end
end

function pixel.Level:update(dt)
    self.sprite_batch:restart()

    for _, map in pairs(self.maps) do
        map:update(dt)
    end

    for _, actor in pairs(self.actors) do
        actor:update(dt, self)
    end
end

function pixel.Level:draw()
    for i, actor in pairs(self.actors) do
        if actor:active() then
            local sprites = actor:draw(dt, self) or {}
            for i, sprite in pairs(sprites) do
                self.sprite_batch:add(sprite)
            end
        end
    end

    if self.debug then
        for i, actor in pairs(self.actors) do
            if actor:active() then
                local sprites = actor:debug_draw() or {}
                for i, sprite in pairs(sprites) do
                    self.sprite_batch:add(sprite)
                end
            end
        end
    end

    if #self.maps > 0 then
        for i, map in pairs(self.maps) do
            self.map_renderer:render(map, self.camera)
        end
    end

    if #self.sprite_batch:sprites() > 0 then
        self.sprite_renderer:render(self.sprite_batch:sprites(), self.atlas_texture, self.camera)
    end
end

-- pixel.Actor
pixel.Actor = {}

function pixel.Actor:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    o:__init(o)

    return o
end

function pixel.Actor:__init(o)
    self.x = o.x or 0.0
    self.y = o.y or 0.0
end

function pixel.Actor:active()
    return true
end

function pixel.Actor:update(dt, level)
end

function pixel.Actor:draw()
end

-- Collision

local collision = {
    rects_overlap = function(r1, r2)
        return (r1.x < r2.x + r2.w) and
                (r1.x + r1.w > r2.x) and
                (r1.y < r2.y + r2.h) and
                (r1.y + r1.h > r2.y)
    end
}

collision.Hitbox = {}

-- Axis-aligned bounding box
function collision.Hitbox:new(x, y, w, h)
    local o = {}
    setmetatable(o, self)
    self.__index = self

    o.rect = { x = x, y = y, w = w, h = h }
    o.last_position = { x = x, y = y }

    return o
end

function collision.Hitbox:as_rect()
    return { x = self.rect.x, y = self.rect.y, w = self.rect.w, h = self.rect.h }
end

function collision.Hitbox:move_by(dx, dy)
    self.last_position.x = self.rect.x
    self.last_position.y = self.rect.y

    self.rect.x = self.rect.x + dx
    self.rect.y = self.rect.y + dy
end

function collision.Hitbox:sweep_rect()
    if self.rect.x == self.last_position.x and self.rect.y == self.last_position.y then
        return self:as_rect()
    end

    local dx = math.abs(self.rect.x - self.last_position.x)
    local dy = math.abs(self.rect.y - self.last_position.y)

    return {
        x = math.min(self.rect.x, self.last_position.x),
        y = math.min(self.rect.y, self.last_position.y),
        w = self.rect.w + dx,
        h = self.rect.h + dy
    }
end

pixel.collision = collision

-- utils


--- - pixel.TileProperties
-- pixel.TileProperties = {}
--
-- function pixel.TileProperties:new(o)
-- o = o or {}
-- setmetatable(o, self)
-- self.__index = self
--
-- o:__init(o)
--
-- return o
-- end
--
-- function pixel.TileProperties:__init(o)
-- if o.solid ~= nil then
-- self.solid = o.solid
-- else
-- self.solid = true
-- end
-- end
--
-- pixel.TileMap_ = pixel.TileMap
--
-- pixel.TileMap = {}
--
-- function pixel.TileMap:new(o)
--
-- end
--
