-- Rect
local inspect = require('inspect')
local Rect = { x = 0, y = 0, w = 0, h = 0 }

function Rect:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end


local function split_rect(tree, block)
    tree.right = { rect = Rect:new { x = tree.rect.x + block.w, y = tree.rect.y + block.h, w = tree.rect.w - block.w, h = block.h } }
    tree.down = { rect = Rect:new { x = tree.rect.x + block.w, y = tree.rect.y + block.h, w = tree.rect.w, h = tree.rect.h - block.h } }
    tree.used = true
    tree.rect.w = block.w
    tree.rect.h = block.h
    return tree
end


local function find_rect(tree, block)
    if tree.used then
        return find_rect(tree.right, block) or find_rect(tree.down, block)
    elseif tree.rect.w >= block.w and tree.rect.h >= block.h then
        return split_rect(tree, block)
    else
        return nil
    end
end

local function pack_rects(rects, w, h)
    local tree = { rect = Rect:new { w = w, h = h } }
    local registry = {}
    local leftover = {}

    for i, r in pairs(rects) do
        local node = find_rect(tree, r)
        if node then
            registry[r] = { x = node.rect.x, y = node.rect.y }
        else
            leftover[#leftover + 1] = r
        end
    end

    return registry, leftover, tree
end


local function pack_rects_array(rects, w, h, d)
    local trees = {}
    for i = 1, d do table.insert(trees, { rect = Rect:new { w = w, h = h } }) end

    local registry = {}
    local leftover = {}

    for i, r in pairs(rects) do
        for layer = 1, d do
            local node = find_rect(trees[layer], r)
            if node then
                registry[r] = { x = node.rect.x, y = node.rect.y, z = layer - 1 }
                break
            elseif (layer == d) then
                table.insert(leftover, r)
            end
        end
    end

    return registry, leftover, trees
end


return {
    pack_rects = pack_rects,
    pack_rects_array = pack_rects_array,
    Rect = Rect
}
