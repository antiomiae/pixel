local function animation(o)
    local frame_list = o.frames

    local out = {}

    for i, frame in pairs(frame_list) do
         table.insert(out, { region_name = frame[1], duration = frame[2] })
    end

    return out
end

return {
    animation = animation
}
