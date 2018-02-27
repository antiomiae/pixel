function ivec2(v)
    return pixel.glm.ivec2.new(v[1], v[2])
end

function vec4(v)
    return pixel.glm.vec4.new(v[1], v[2], v[3], v[4])
end

function pixel.load_map(s)
    tm = pixel.TileMap.new()
    tm:load(s)
    return tm
end

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

local map = pixel.load_map("assets/traps_1.tmx")

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
