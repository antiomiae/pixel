

#ifndef PIXEL_PACK_H
#define PIXEL_PACK_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <pixel/error.h>
#include <pixel/error.h>

namespace pixel::pack
{

using namespace std;

struct PackParams
{
    unsigned x{}, y{}, z{};
    bool flipped;
    PackParams() = default;

    PackParams(unsigned x, unsigned y, unsigned z, bool flipped)
        : x{x}, y{y}, z{z}, flipped{flipped}
    {
    };
};

struct PackNode
{
    unsigned w{}, h{}, x{}, y{};
    bool used{false};
    bool flipped{false};
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

    bool can_fit(unsigned _w, unsigned _h)
    {
        return std::max(w, h) >= std::max(_w, _h) && std::min(w, h) >= std::min(_w, _h);
    }

    bool should_flip(unsigned _w, unsigned _h)
    {
        return ((_w > _h && _w <= h) || ((_w > w || _h > h) && (_h <= w && _w <= h)));
    }
};

int free_area(const PackNode* tree);

template<typename R>
PackNode* split_rect(PackNode* tree, const R& block)
{
    unsigned w, h;
    w = block.w;
    h = block.h;

    if (tree->should_flip(w, h)) {
        std::swap(w, h);
        tree->flipped = true;
    }
    tree->right = new PackNode(tree->x + w, tree->y, tree->w - w, h);
    tree->down = new PackNode(tree->x, tree->y + h, tree->w, tree->h - h);
    tree->w = w;
    tree->h = h;
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
        if (auto p = find_rect(tree->right, block)) {
            return p;
        }
        return find_rect(tree->down, block);
    } else if (tree->can_fit(block.w, block.h)) {
        return split_rect(tree, block);
//    } else if (tree->w >= block.h && tree->h >= block.w) {
//        auto b = block;
//        auto t = b.w;
//        b.w = b.h;
//        b.h = t;
//        return split_rect(tree, b);
    }

    return nullptr;
}


template<typename R>
pair<vector<pair<R, PackParams> >, vector<R> > pack_rects_array(vector<R>& blocks, unsigned w, unsigned h, unsigned d)
{
    argument_error_if(w == 0 || h == 0 || d == 0, "All dimensions must be > 0");

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
                if (!node->flipped) {
                    if (node->x + blocks[i].w > w) {
                        pixel_error("Placement violates bounds: exceeds width.");
                    }
                    if (node->y + blocks[i].h > h) {
                        pixel_error("Placement violates bounds: exceeds height.");
                    }
                } else {
                    if (node->x + blocks[i].h > w) {
                        pixel_error("Placement violates bounds: exceeds width.");
                    }
                    if (node->y + blocks[i].w > h) {
                        pixel_error("Placement violates bounds: exceeds height.");
                    }
                }

                registry.push_back({blocks[i], PackParams{node->x, node->y, layer, node->flipped}});

                break;
            } else if (layer == d - 1) {
                leftover.push_back(blocks[i]);
            }
        }
    }

//    for (const auto& node : trees) {
//        if (!node.used) {
//            break;
//        }
//
//        float remaining = free_area(&node);
//        float ratio = ((w * h) - remaining) / (w * h);
//
//        cout << "Layer pack ratio: " << ratio * 100 << '%' << endl;
//    }

    return {registry, leftover};
}

}

#endif //PIXEL_PACK_H
