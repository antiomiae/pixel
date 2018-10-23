

#ifndef PIXEL_MAIN_ANIMATION_H
#define PIXEL_MAIN_ANIMATION_H

namespace pixel
{

class Animation
{
    virtual void update(float dt) = 0;
    virtual bool is_complete() = 0;

    virtual ~Animation() {};
};

}
#endif //PIXEL_MAIN_ANIMATION_H
