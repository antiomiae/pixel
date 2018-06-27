#include "../setup.h"
#include <pixel/graphics/renderers/line_renderer.h>

namespace
{

TEST_CASE("LineRenderer")
{
    REQUIRE_NOTHROW([] {
        pixel::graphics::renderers::LineRenderer line_renderer{};
    });
}

}

