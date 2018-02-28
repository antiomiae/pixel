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

local renderer = pixel.TileMapRenderer.new()

local obs = {}
table.insert(obs, map)

app:set_tick_callback(function ()
    for i, o in pairs(obs) do
        o:update(1/60.0)
    end

    renderer:render(map, app:render_context())
end)

app:run()
