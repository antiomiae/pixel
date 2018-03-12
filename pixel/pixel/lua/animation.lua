local function animation(o)
    local frame_list = o.frames

    local out = {}
    out.frames = {}

    out.name = assert(o.name, "Must specify 'name' property for animation")
    out.timescale = o.timescale

    for i, frame in ipairs(frame_list) do
         table.insert(out.frames, { region_name = frame[1], duration = frame[2] })
    end

    return out
end

-- Load animation file at path, using atlas
function pixel.load_animations(path, atlas)
    -- backup global state
    local bak = _G["animation"]

    local data = {}

    _G["animation"] = function (o)
        table.insert(data, animation(o))
    end

    -- should call animation and store results in data
    dofile(path)

    assert(#data > 0, "No animations loaded from file")

    -- restore global state
    _G["animation"] = bak


    local out = {}

    for i, anim_data in ipairs(data) do
        local anim = pixel.SpriteAnimation:new()
        anim:set_time_scale(anim_data.timescale or 1000)
        --
        for j, frame_data in ipairs(anim_data.frames) do
            local tex_region = atlas:lookup(frame_data.region_name)

            anim:add_frame(tex_region, frame_data.duration)
        end

        out[anim_data.name] = anim
    end

    return out
end
