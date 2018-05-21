//
//

#include "render_context.h"

using namespace pixel;


RenderContext::RenderContext(const glm::ivec2& real_size, const glm::ivec2& virtual_size, const glm::vec4& color)
    : window_size{real_size},
      virtual_window_size{virtual_size},
      default_clear_color{color}
{
}


glm::mat4 RenderContext::projection() const
{
    return glm::ortho(0.0f, (float) virtual_window_size.x, 0.0f, (float) virtual_window_size.y);
}
