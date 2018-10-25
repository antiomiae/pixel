

#ifndef PIXEL_MAIN_LAMBDA_ANIMATION_H
#define PIXEL_MAIN_LAMBDA_ANIMATION_H

#include <functional>
#include "animation.h"

namespace pixel
{

class LambdaAnimation : public Animation
{
public:
    explicit LambdaAnimation(std::function<bool(float)> lambda);

    virtual void update(float dt) override;
    virtual bool is_complete() override;
private:
    std::function<bool(float)> lambda_;
    bool last_result_{false};
};

}

#endif //PIXEL_MAIN_LAMBDA_ANIMATION_H
