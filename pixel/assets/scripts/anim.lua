--
require 'pixel'

local app = pixel.App.create {
    width = 640,
    height = 480,
    background_color = { 0.5, 0.5, 0.5, 1.0 },
    pixel_scale = 2.0
}

app:init(0)

local SPRITES = {
    ["assets/sonic.png"] = "sonic",
    ["assets/sample_sprites/5.png"] = "joanna",
    ["assets/sample_sprites/6.png"] = "joanna2",
    ["assets/sprites/cat_Animation 1_0.png"] = "cat0",
    ["assets/sprites/cat_Animation 1_2.png"] = "cat2",
    ["assets/sprites/cat_Animation 1_3.png"] = "cat3",
    ["assets/sprites/cat_Animation 1_4.png"] = "cat4",
    ["assets/sprites/spy_idle_0.png"] = "spy_idle_0",
    ["assets/sprites/spy_idle_1.png"] = "spy_idle_1",
    ["assets/sprites/spy_idle_2.png"] = "spy_idle_2",
    ["assets/sprites/spy_idle_3.png"] = "spy_idle_3",
}

local current_level = pixel.Level:new()
current_level.camera:lock_x(true)

current_level:load_sprites(SPRITES)

current_level:add_map(pixel.load_map("assets/map.tmx"))

current_level:add_animation('assets/animations/sonic.lua')
current_level:add_animation('assets/animations/cat.lua')
current_level:add_animation('assets/animations/spy.lua')

print(pixel.inspect(current_level))

local sonic = pixel.Actor:new {
    x = 0,
    y = 0,
    vx = 50,
    vy = 50,
    animation = current_level.animations.spy_idle:copy(),
    sprite = pixel.Sprite.new(),
    dir = 1,
    state = "falling"
}

function sonic:update(dt, level)
    self.animation:update(dt)
    self.animation:update_sprite(self.sprite)

    self.x = self.x + self.vx * dt
    self.y = self.y + self.vy * dt

    level.camera:follow(self.x, self.y)

    self.sprite.x = self.x
    self.sprite.y = self.y
    self.sprite:flip_h(self.dir ~= 1)
end

function sonic:draw()
    return {self.sprite}
end

current_level:add_actor(sonic)

app:set_tick_callback(function ()
    current_level:update(1/60.0)
    current_level:draw()
end)

app:run()
