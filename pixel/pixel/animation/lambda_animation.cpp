

#include "lambda_animation.h"

namespace pixel {
LambdaAnimation::LambdaAnimation(std::function<bool(float)> lambda)
: lambda_(std::move(lambda))
{
}

void LambdaAnimation::update(float dt)
{
    last_result_ = lambda_(dt);
}

bool LambdaAnimation::is_complete()
{
    return last_result_;
}

}
