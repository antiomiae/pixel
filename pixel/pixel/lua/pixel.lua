pixel.inspect = require('inspect')
pixel.animation = require('animation').animation

--
function pixel.load_map(s)
    local tm = pixel.TileMap.new()
    tm:load(s)
    return tm
end

-- Load animation file at path, using atlas
function pixel.load_animation(path, atlas)
    -- backup global state
    local bak = _G["animation"]

    local data = {}

    _G["animation"] = function (o)
        table.insert(data, pixel.animation(o))
    end

    -- should call animation and store results in data
    dofile(path)

    assert(data)

    -- restore global state
    _G["animation"] = bak

    local anim = pixel.SpriteAnimation:new()

    for i, anim_data in pairs(data) do
        --
        local tex_region = atlas.lookup(anim_data.region_name)
        anim.add_frame(tex_region, anim_data.duration)
    end

    return data
end
