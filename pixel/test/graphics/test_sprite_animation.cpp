#include <pixel/graphics/graphics.h>
#include "test/setup.h"

namespace
{

using namespace pixel;
using namespace pixel::graphics;

TEST_CASE("SpriteAnimation")
{
    SpriteAnimation anim{};

    anim.add_frame({0, 0, 10, 10, 0, 0}, 10);
    anim.add_frame({10, 0, 10, 10, 0, 0}, 10);
    anim.add_frame({20, 0, 10, 10, 0, 0}, 10);
    anim.add_frame({30, 0, 10, 10, 0, 0}, 10);

    // set time scale so that animation durations represent milliseconds
    anim.set_time_scale(1000.0);

    anim.reset();

    REQUIRE(anim.current_frame().texture_region.x == 0);

    anim.update(0.010);
    REQUIRE(anim.current_frame().texture_region.x == 10);

    anim.update(0.010);
    REQUIRE(anim.current_frame().texture_region.x == 20);

    anim.update(0.010);
    REQUIRE(anim.current_frame().texture_region.x == 30);

    anim.update(0.010);
    REQUIRE(anim.current_frame().texture_region.x == 0);
}

};

