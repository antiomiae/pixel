--
require 'pixel'

local tilemap = require 'tilemap'

W = 320 * 3
H = 224 * 3

local app = pixel.App.create {
    width = W,
    height = H,
    background_color = { 0.1, 0.1, 0.1, 1.0 },
    pixel_scale = 2.0
}

app:init(0)

pixel.Keyboard.register_callback(app:window())

local KEYS = {
    W = string.byte('W'),
    A = string.byte('A'),
    S = string.byte('S'),
    D = string.byte('D'),
}

local SPRITES = {
    ["assets/sprites/spy_idle_0.png"] = "spy_idle_0",
    ["assets/sprites/spy_idle_1.png"] = "spy_idle_1",
    ["assets/sprites/spy_idle_2.png"] = "spy_idle_2",
    ["assets/sprites/spy_idle_3.png"] = "spy_idle_3",
    ["assets/sprites/spy_walk_0.png"] = "spy_walk_0",
    ["assets/sprites/spy_walk_1.png"] = "spy_walk_1",
    ["assets/sonic.png"] = "sonic"
}

local current_level = pixel.Level:new()
--current_level.camera:lock_x(true)
current_level.camera:set_window_size(math.floor(W / 3), math.floor(H / 3))

local render_target = pixel.OffscreenRenderTarget.new()
render_target:set_window_size(W // 3, H // 3)

current_level:load_sprites(SPRITES)

current_level:add_map(pixel.load_map("assets/map.tmx"))

local simple_tilemap = tilemap.TileMap:new({ tile_map = current_level.maps[1] })

current_level:add_animation('assets/animations/spy.lua')

local sonic = pixel.Actor:new {
    x = 0,
    y = 0,
    vx = 0,
    vy = 0,
    --animation = current_level.animations.spy_idle:copy(),
    sprite = pixel.Sprite:new(),
    dir = 1,
    state = nil,
    last_state = nil
}

sonic.sprite.texture_region = current_level.atlas:lookup("sonic")

-- updates state value
-- returns true if passed state is different than current state
function sonic:transition_state(new_state)
    self.last_state = self.state

    if new_state == self.state then
        return false
    end

    self.state = new_state

    return true
end

function sonic:update(dt, level)
    self.frame = (self.frame or 0) + 1

    if self.animation then
        self.animation:update(dt)
        self.animation:update_sprite(self.sprite)
    end

    if pixel.Keyboard.keymap[KEYS.W] then
        self.vy = 100
    elseif pixel.Keyboard.keymap[KEYS.S] then
        self.vy = -100
    else
        self.vy = 0
    end

    if pixel.Keyboard.keymap[KEYS.A] then
        self.vx = -100
        self.dir = -1
    elseif pixel.Keyboard.keymap[KEYS.D] then
        self.vx = 100
        self.dir = 1
    else
        self.vx = 0
    end

--
--    if math.abs(self.vx) > 0 or math.abs(self.vy) > 0 then
--        if self:transition_state("walk") then
--            self.animation = current_level.animations.spy_walk:copy()
--        end
--    else
--        if self:transition_state("idle") then
--            self.animation = current_level.animations.spy_idle:copy()
--        end
--    end

    self.x = self.x + self.vx * dt
    self.y = self.y + self.vy * dt

    if simple_tilemap:has_coord(self) then
        local tile = simple_tilemap:tile_at_coord(self, 3)

        if tile.name then
            print(string.format("tile name = %q", tile.name))
        end
    end

    level.camera:follow(self.x, self.y)

    self.i = self.i or 0.0
    self.i = self.i + 0.01
    if self.i > 1.0 then
        self.i = 0.0
    end

--    self.sprite.color.x = math.sin(self.i * 20)/2 + 0.5
--    self.sprite.color.y = math.cos(self.i * 30)/2 + 0.5
--    self.sprite.color.z = math.sin(self.i * 10 + 3.14/4)/2 + 0.5

    self.sprite.color.x = 1 - self.i
    --self.sprite.color.y = 1 - self.i
    self.sprite.color.z = 1 - self.i

    self.sprite.tint.x = self.i
    self.sprite.tint.y = self.i
    self.sprite.tint.z = self.i

    self.sprite.x = self.x
    self.sprite.y = self.y
    self.sprite:flip_h(self.dir ~= 1)
end

sonic.x = W / 6 / 2
sonic.y = H / 6 / 2

function sonic:draw()
    return { self.sprite }
end

current_level:add_actor(sonic)

local function draw()
    render_target:activate()

    pixel.gl.glClear(pixel.gl.GL_COLOR_BUFFER_BIT)

    current_level:draw()

    render_target:deactivate()

    pixel.gl.glClear(pixel.gl.GL_COLOR_BUFFER_BIT)

    render_target:draw(pixel.glm.ivec4:new(0, 0, W, H))
end

app:set_tick_callback(function()
    current_level:update(1 / 60.0)
    draw()
end)

app:run()
