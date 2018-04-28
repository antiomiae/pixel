#include "../setup.h"
#include <pixel/renderers/line_renderer.h>

namespace
{

TEST(LineRenderer, constructor)
{
    ASSERT_NO_THROW(pixel::renderers::LineRenderer line_renderer{});
}

}

