--
function pixel.load_map(s)
    tm = pixel.TileMap.new()
    tm:load(s)
    return tm
end
