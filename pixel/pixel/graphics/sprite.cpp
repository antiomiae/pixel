//
//

#include "sprite.h"

void pixel::graphics::Sprite::flip_h(bool set)
{
    if (set) {
        flip_flags |= kHorizontal;
    } else {
        flip_flags &= ~kHorizontal;
    }
}

bool pixel::graphics::Sprite::flip_h()
{
    return flip_flags & kHorizontal;
}

void pixel::graphics::Sprite::flip_v(bool set)
{

    if (set) {
        flip_flags |= kVertical;
    } else {
        flip_flags &= ~kVertical;
    }
}

bool pixel::graphics::Sprite::flip_v()
{
    return flip_flags & kHorizontal;
}

void pixel::graphics::Sprite::flip_d(bool set)
{
    if (set) {
        flip_flags |= kDiagonal;
    } else {
        flip_flags &= ~kDiagonal;
    }
}

bool pixel::graphics::Sprite::flip_d()
{
    return flip_flags & kHorizontal;
}
