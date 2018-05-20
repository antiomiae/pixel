//
//

#include "sprite.h"

namespace pixel::graphics
{


void Sprite::flip_h(bool set)
{
    if (set) {
        flip_flags |= FlipFlag::kHorizontal;
    } else {
        flip_flags &= ~FlipFlag::kHorizontal;
    }
}

bool Sprite::flip_h()
{
    return flip_flags & FlipFlag::kHorizontal;
}

void Sprite::flip_v(bool set)
{

    if (set) {
        flip_flags |= FlipFlag::kVertical;
    } else {
        flip_flags &= ~FlipFlag::kVertical;
    }
}

bool Sprite::flip_v()
{
    return flip_flags & FlipFlag::kHorizontal;
}

void Sprite::flip_d(bool set)
{
    if (set) {
        flip_flags |= FlipFlag::kDiagonal;
    } else {
        flip_flags &= ~FlipFlag::kDiagonal;
    }
}

bool Sprite::flip_d()
{
    return flip_flags & FlipFlag::kHorizontal;
}

}
