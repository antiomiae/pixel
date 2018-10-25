

#ifndef PIXEL_MAIN_ANIMATION_GROUP_H
#define PIXEL_MAIN_ANIMATION_GROUP_H

#include "animation.h"
#include <vector>
#include <memory>

using namespace std;

namespace pixel
{

class AnimationGroup : public Animation
{
public:
    using animation_ptr = unique_ptr<Animation>;

    void update(float dt) override;
    bool is_complete() override;
    void add_animation(animation_ptr&& animation);

private:
    vector<animation_ptr> animations_;
};

}
#endif //PIXEL_MAIN_ANIMATION_GROUP_H
