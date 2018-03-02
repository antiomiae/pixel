

#include "pack.h"

int pixel::pack::free_area(const pixel::pack::PackNode* tree)
{
    if (tree == nullptr) {
        return 0;
    }

    if (tree->used) {
        return free_area(tree->right) + free_area(tree->down);
    } else {
        return tree->w * tree->h;
    }
}
