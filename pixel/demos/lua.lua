require 'pixel'

local app = pixel.App.create {
    width = 640,
    height = 480,
    background_color = { 0.5, 0.5, 0.5, 1.0 },
    pixel_scale = 2.0
}

app:init(0)

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

local j = 0.0

app:set_tick_callback(function ()
    for i, o in pairs(obs) do
        o:update(1/60.0)
    end

    j = j + 0.01

    camera:center_at(math.cos(j) * 50 + 320/2, math.sin(j) * 50 + 240 / 2)

    map_renderer:render(map, camera)
end)

app:run()
