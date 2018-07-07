#include "../setup.h"
#include <pixel/renderers/renderers.h>

namespace
{

TEST_CASE("LineRenderer")
{
    REQUIRE_NOTHROW([] {
        pixel::renderers::LineRenderer line_renderer{};
    });
}

}

