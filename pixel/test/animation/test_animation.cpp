#include <pixel/animation/animation_group.h>
#include <pixel/animation/lambda_animation.h>
#include <test/setup.h>

namespace
{

TEST_CASE("AnimationGroup")
{
    pixel::AnimationGroup animation_group;

    float opacity = 0;
    float speed = 0;
    float x = 0;

    animation_group.add_animation(make_unique<pixel::LambdaAnimation>([&] (float dt) {
        static float time = 0.0f;
        time += dt;

        opacity += 0.1;

        return (time >= 1.0f);
    }));

    animation_group.add_animation(make_unique<pixel::LambdaAnimation>([&] (float dt) {
        const float ds = 0.1f;
        speed += ds;
        return (speed >= 2.0f);
    }));

    animation_group.add_animation(make_unique<pixel::LambdaAnimation>([&] (float dt) {
        static float time = 0.0f;
        time += dt;
        x = time;
        return (time >= 3.0f);
    }));

    REQUIRE(animation_group.is_complete() == false);

    while (!animation_group.is_complete()) {
        animation_group.update(1/60.0f);
    }

    REQUIRE(opacity > 0);
    REQUIRE(speed >= 2.0f);
    REQUIRE(x >= 3.0f);
}

}
