#include "animation_group.h"
#include <algorithm>

namespace pixel
{

void AnimationGroup::update(float dt)
{
    auto last = std::remove_if(begin(animations_), end(animations_), [=](animation_ptr& a) -> bool {
        a->update(dt);
        return a->is_complete();
    });

    animations_.resize(std::distance(begin(animations_), last));
}

void AnimationGroup::add_animation(animation_ptr&& animation)
{
    animations_.push_back(std::move(animation));
}

bool AnimationGroup::is_complete()
{
    auto last = std::remove_if(
        begin(animations_),
        end(animations_),
        [&](const animation_ptr& a) -> bool {
            return a->is_complete();
        }
    );
    animations_.resize(std::distance(begin(animations_), last));

    return animations_.empty();
}

}
