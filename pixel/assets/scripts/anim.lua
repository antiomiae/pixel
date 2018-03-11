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
    ["assets/sprites/cat_Animation 1_0.png"] = "cat0",
    ["assets/sprites/cat_Animation 1_2.png"] = "cat2",
    ["assets/sprites/cat_Animation 1_3.png"] = "cat3",
    ["assets/sprites/cat_Animation 1_4.png"] = "cat4"
}

-- Not possible yet
--local app = pixel.App.new {
--    ivec2 { 320, 240 },
--    vec4 { 0.5, 0.5, 0.5, 1.0 },
--    1.0
--}



local current_level = pixel.Level:new()


current_level:load_sprites(SPRITES)

current_level:add_map(pixel.load_map("assets/map.tmx"))
current_level:add_animation('assets/animations/sonic.lua')
current_level:add_animation('assets/animations/cat.lua')

print(pixel.inspect(current_level))

local sonic = {
    x = 0,
    y = 0,
    vx = 50,
    vy = 50,
    animation = current_level.animations.cat:copy(),
    sprite = pixel.Sprite.new()
}

current_level.camera:lock_x(true)

function sonic:update(dt)
    self.animation:update(dt)
    self.animation:update_sprite(self.sprite)

    self.x = self.x + self.vx * dt
    self.y = self.y + self.vy * dt

    if self.x + self.sprite.texture_region.w >= 320 then
        self.vx = - math.abs(self.vx)
    elseif self.x <= 0 then
        self.vx = math.abs(self.vx)
    end

    if self.y + self.sprite.texture_region.h >= 240 then
        self.vy = - math.abs(self.vy)
    elseif self.y <= 0 then
        self.vy = math.abs(self.vy)
    end

    current_level.camera:follow(self.x, self.y)

    self.sprite.x = self.x
    self.sprite.y = self.y

    current_level.sprite_batch:add(self.sprite)
end

current_level:add_actor(sonic)

app:set_tick_callback(function ()
    current_level:update(1/60.0)

    current_level:draw()


--
--
--    sprite_renderer.batch:add(sonic_sprite)
--
--
--    sprite_renderer:render(sprite_renderer.batch:sprites(), atlas_texture, camera)
--
--
--    sprite_renderer.batch:restart()
--
--    map_renderer:render(map, camera)
end)

app:run()
