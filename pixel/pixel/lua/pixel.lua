pixel.inspect = require('inspect')

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

