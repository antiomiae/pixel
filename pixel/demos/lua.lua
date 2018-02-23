function ivec2(v)
    return pixel.glm.ivec2.new(v[1], v[2])
end

function vec4(v)
    return pixel.glm.vec4.new(v[1], v[2], v[3], v[4])
end

local app = pixel.App.create {
    width = 620,
    height = 480,
    background_color = { 0.5, 0.5, 0.5, 1.0 },
    pixel_scale = 2.0
}

-- Not possible yet
--local app = pixel.App.new {
--    ivec2 { 320, 240 },
--    vec4 { 0.5, 0.5, 0.5, 1.0 },
--    1.0
--}

app:init(0)

app:set_tick_callback(function () end)

app:run()
