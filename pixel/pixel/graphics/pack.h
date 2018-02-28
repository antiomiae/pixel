

#ifndef PIXEL_PACK_H
#define PIXEL_PACK_H

#include <unordered_map>
#include <vector>
#include <pixel/error.h>

namespace pixel::pack
{

using namespace std;

/*
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

 */

struct PackParams
{
    unsigned x{}, y{}, z{};
    PackParams() = default;


    PackParams(unsigned x, unsigned y, unsigned z)
        : x{x}, y{y}, z{z}
    {

    };
};

struct PackNode
{
    unsigned w{}, h{}, x{}, y{};
    bool used{false};
    PackNode* right{nullptr};
    PackNode* down{nullptr};


    PackNode(unsigned x, unsigned y, unsigned w, unsigned h)
        : x{x}, y{y}, w{w}, h{h}
    { }


    PackNode() = default;


    ~PackNode()
    {
        if (right) {
            delete right;
            right = nullptr;
        }
        if (down) {
            delete down;
            down = nullptr;
        }
    }
};


template<typename R>
PackNode* split_rect(PackNode* tree, const R& block)
{
    tree->right = new PackNode(tree->x + block.w, tree->y, tree->w - block.w, block.h);
    tree->down = new PackNode(tree->x, tree->y + block.h, tree->w, tree->h - block.h);
    tree->w = block.w;
    tree->h = block.h;
    tree->used = true;
    return tree;
}


template<typename R>
PackNode* find_rect(PackNode* tree, const R& block)
{
    if (tree == nullptr) {
        return nullptr;
    }

    if (tree->used) {
        if (auto node = find_rect(tree->right, block)) {
            return node;
        }

        return find_rect(tree->down, block);
    } else if (tree->w >= block.w && tree->h >= block.h) {
        return split_rect(tree, block);
    }

    return nullptr;
}


template<typename R>
pair<vector<pair<R, PackParams> >, vector<R> > pack_rects_array(vector<R>& blocks, unsigned w, unsigned h, unsigned d)
{
    argument_error_if(w == 0 || h == 0 || d == 0, "Only non-zero dimensions allowed");

    vector<PackNode> trees(d);

    for (auto& node : trees) {
        node = PackNode(0, 0, w, h);
    }

    vector<pair<R, PackParams> > registry{};
    vector<R> leftover{};

    for (auto i = 0u; i < blocks.size(); ++i) {
        for (auto layer = 0u; layer < d; ++layer) {
            auto node = find_rect(&trees[layer], blocks[i]);

            if (node) {
                registry.push_back({blocks[i], PackParams{node->x, node->y, layer}});
                break;
            } else if (layer == d - 1) {
                leftover.push_back(blocks[i]);
            }
        }
    }

    return {registry, leftover};
}

}

#endif //PIXEL_PACK_H
