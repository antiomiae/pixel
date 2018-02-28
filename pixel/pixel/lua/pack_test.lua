local pack = require 'pack'
local inspect = require('inspect')

local map, leftovers, tree = pack.pack_rects({{w=10,h=10}, {w=8,h=9}, {w=3,h=4}}, 17, 30)

assert(map)
assert(#leftovers == 0)

local map, leftovers, tree = pack.pack_rects_array(
    {{w=15,h=20}, {w=10,h=30},{w=10,h=10}, {w=8,h=9}, {w=9,h=8}, {w=7,h=8}, {w=3,h=4}},
    17, 30, 10
)

assert(map)
assert(#leftovers == 0)

print(inspect(map))
--print(inspect(tree))
