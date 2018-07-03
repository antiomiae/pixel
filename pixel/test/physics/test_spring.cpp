#include <pixel/physics/constraints.h>
#include "../setup.h"

namespace
{

using namespace pixel::physics;

TEST_CASE("Spring")
{
    Spring spring(10.0f);

    REQUIRE(spring.force(10.0f) == Approx(100.0f));

    auto r = glm::vec2(1.0 / sqrt(2.0), 1.0 / sqrt(2.0));

    REQUIRE(spring.force(glm::vec2(0.f, 0.f), r) == (r * 10.0f));
}

}

