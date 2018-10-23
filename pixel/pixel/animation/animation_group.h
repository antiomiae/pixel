

#ifndef PIXEL_MAIN_ANIMATION_GROUP_H
#define PIXEL_MAIN_ANIMATION_GROUP_H

#include "animation.h"
#include <vector>
#include <memory>

using namespace std;

namespace pixel
{

class AnimationGroup
{
    using animation_ptr = unique_ptr<Animation>;
private:
    vector<animation_ptr> animations_;
};

}
#endif //PIXEL_MAIN_ANIMATION_GROUP_H
