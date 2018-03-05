//
//

#include "render_context.h"

using namespace pixel;


RenderContext::RenderContext(glm::ivec2 size, glm::vec4 color, float scale)
    : window_size{size},
      default_clear_color{color},
      pixel_scale{scale}
{
}


glm::mat4 RenderContext::projection() const
{
    return glm::ortho(0.0f, (float) window_size.x / pixel_scale, 0.0f, (float) window_size.y / pixel_scale);
}
