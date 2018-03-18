--
require 'pixel'

W = 1400
H = 850

local app = pixel.App.create {
    width = W,
    height =  H,
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
    ["assets/sprites/spy_walk_1.png"] = "spy_walk_1"
}

local current_level = pixel.Level:new()
--current_level.camera:lock_x(true)
current_level.camera:set_window_size(math.floor(W/3), math.floor(H/3))

current_level:load_sprites(SPRITES)

current_level:add_map(pixel.load_map("assets/map.tmx"))

current_level:add_animation('assets/animations/spy.lua')

local sonic = pixel.Actor:new {
    x = 0,
    y = 0,
    vx = 0,
    vy = 0,
    animation = current_level.animations.spy_idle:copy(),
    sprite = pixel.Sprite:new(),
    dir = 1,
    state = nil,
    last_state = nil
}

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
    self.animation:update(dt)
    self.animation:update_sprite(self.sprite)

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

    if math.abs(self.vx) > 0 or math.abs(self.vy) > 0 then
        if self:transition_state("walk") then
            self.animation = current_level.animations.spy_walk:copy()
        end
    else
        if self:transition_state("idle") then
            self.animation = current_level.animations.spy_idle:copy()
        end
    end




    self.x = self.x + self.vx * dt
    self.y = self.y + self.vy * dt

    level.camera:follow(self.x, self.y)

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

app:set_tick_callback(function()
    current_level:update(1 / 60.0)
    current_level:draw()
end)

app:run()
