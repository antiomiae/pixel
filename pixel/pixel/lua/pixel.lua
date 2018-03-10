pixel.inspect = require('inspect')

--
function pixel.load_map(s)
    local tm = pixel.TileMap.new()
    tm:load(s)
    return tm
end
