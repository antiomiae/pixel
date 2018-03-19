require 'pixel'

local app = pixel.App.create {
    width = 320,
    height = 240,
    background_color = { 0.5, 0.5, 0.5, 1.0 },
    pixel_scale = 2.0
}

app:init(0)

pixel.Keyboard.register_callback(app:window())

-- Not possible yet
--local app = pixel.App.new {
--    ivec2 { 320, 240 },
--    vec4 { 0.5, 0.5, 0.5, 1.0 },
--    1.0
--}

local map = pixel.load_map("assets/map.tmx")

local map_renderer = pixel.TileMapRenderer.new()

local obs = {}
table.insert(obs, map)

local camera = pixel.Camera:new()

local atlas = pixel.TextureAtlas.new(pixel.glm.uvec3.new(512, 512, 10))

atlas:start_batch()
atlas:add_image("assets/sonic.png")
atlas:stop_batch()

camera:set_window_size(320, 240)
camera:center_at(0, 0)
--camera:position_at(0, 0)

local j = 0.0

local KEYS = {
    W = string.byte('W'),
    A = string.byte('A'),
    S = string.byte('S'),
    D = string.byte('D'),
}

app:set_tick_callback(function ()
    for i, o in pairs(obs) do
        o:update(1/60.0)
    end

    j = j + 0.01

    local tx = 0.0
    local ty = 0.0

    if pixel.Keyboard.keymap[KEYS.W] then
        ty = 1.0
    elseif pixel.Keyboard.keymap[KEYS.S] then
        ty = -1.0
    end

    if pixel.Keyboard.keymap[KEYS.A] then
        tx = -1.0
    elseif pixel.Keyboard.keymap[KEYS.D] then
        tx = 1.0
    end

    camera:translate(tx, ty)

    camera:scale(0.2*math.sin(j) + 1.0)

    map_renderer:render(map, camera)
end)

app:run()
