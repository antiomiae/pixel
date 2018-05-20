#include "../setup.h"
#include <pixel/graphics/renderers/line_renderer.h>

namespace
{

TEST(LineRenderer, constructor)
{
    ASSERT_NO_THROW(pixel::graphics::renderers::LineRenderer line_renderer{});
}

}

