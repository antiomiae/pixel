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

current_level:add_animation('assets/animations/spy.lua')

local sonic = pixel.Actor:new {
    --animation = current_level.animations.spy_idle:copy(),
    sprite = pixel.Sprite:new(),
    dir = 1,
    state = nil,
    last_state = nil,
    hitbox = pixel.collision.Hitbox:new(0, 0, 20, 20)
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

    self.hitbox:move_by(self.vx * dt, self.vy * dt)
    self:check_collision(level)

    level.camera:follow(self.hitbox.rect.x, self.hitbox.rect.y)

    self.sprite.x = self.hitbox.rect.x
    self.sprite.y = self.hitbox.rect.y
    self.sprite:flip_h(self.dir ~= 1)
end

function sonic:check_collision(level)
    local check_rect = self.hitbox:sweep_rect()

    -- fetch tiles that could possibly be touched
    local overlapping_tiles = level.map_wrappers[1]:get_overlapping_tiles(check_rect, 3)

    local tile_size = level.map_wrappers[1].tile_size

    local sides_colliding = {
        top = nil,
        bottom = nil,
        right = nil,
        left = nil
    }

    -- fake it
    self.hitbox.rect.x = -10
    self.hitbox.rect.y = -10
    self.hitbox:move_by(10, 10)

    -- distance we're trying to move
    local dx = self.hitbox.rect.x - self.hitbox.last_position.x
    local dy = self.hitbox.rect.y - self.hitbox.last_position.y

    local step_dir = {
        x = math.sign(dx),
        y = math.sign(dy)
    }

    local test_rect = {
        x = self.hitbox.last_position.x,
        y = self.hitbox.last_position.y,
        w = self.hitbox.rect.w,
        h = self.hitbox.rect.h
    }

    local test_point = { x = test_rect.x, y = test_rect.y }

    if step_dir.x == 1 then
        test_point.x = test_point.x + test_rect.w
    end

    if step_dir.y == 1 then
        test_point.y = test_point.y + test_rect.h
    end

    local function containing_tile(pt, sz)
        return { x = pt.x // sz.x, y = pt.y // sz.y }
    end

    while dx ~= 0 or dy ~= 0 do
        local start_tile = containing_tile(test_point, tile_size)
        local xtime, ytime

        if dx > 0 then
            print(string.format("test_point = %s", pixel.inspect(test_point)))
            local tile_edge = (start_tile.x + 1) * tile_size.x - 1
            xtime = (tile_edge - test_point.x) / dx
            print(string.format("xtime = %f", xtime))
        elseif dx < 0 then
            local tile_edge = start_tile.x * tile_size.x + 1
            xtime = (test_point.x - tile_edge) / -dx
        end

        if dy > 0 then
            local tile_edge = (start_tile.y + 1) * tile_size.y - 1
            ytime = (tile_edge - test_point.y) / dy
        elseif dy < 0 then
            local tile_edge = start_tile.y * tile_size.y + 1
            ytime = (test_point.y - tile_edge) / -dy
        end

        if ytime or xtime then
            local min_time = math.min(xtime or math.huge, ytime or math.huge)
            if min_time == math.huge then
                break
            end
            break
        else
            break
        end
    end

    print(string.format("dx = %f, dy = %f, xtime = %f, time = %f", dx, dy, xtime or -1, ytime or -1))
end

sonic.x = W / 6 / 2
sonic.y = H / 6 / 2

function sonic:draw()
    return { self.sprite }
end

function sonic:debug_draw()
    if not self.hitbox_sprite then
        local hitbox_sprite = pixel.Sprite:new()

        hitbox_sprite.color = pixel.glm.vec4.new(0.0, 0.0, 0.0, 0.5)
        hitbox_sprite.tint = pixel.glm.vec4.new(1.0, 0.0, 0.0, 1.0)
        self.hitbox_sprite = hitbox_sprite
    end

    self.hitbox_sprite.x = self.hitbox.rect.x
    self.hitbox_sprite.y = self.hitbox.rect.y
    self.hitbox_sprite.texture_region.x = -1
    self.hitbox_sprite.texture_region.w = self.hitbox.rect.w
    self.hitbox_sprite.texture_region.h = self.hitbox.rect.h

    return { self.hitbox_sprite }
end

function math.sign(x)
    if x == 0 then
        return 0
    elseif x > 0 then
        return 1
    else
        return -1
    end
end

current_level:add_actor(sonic)

current_level.debug = true

local Collision = {}

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
