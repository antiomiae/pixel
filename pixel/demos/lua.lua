--

debug = require('debug')

local app = pixel.App.create({
    width = 320,
    height = 240,
    background_color = { 0.5, 0.5, 0.5, 1.0 },
    pixel_scale = 1.0
})

app:init(0)

app:run()
