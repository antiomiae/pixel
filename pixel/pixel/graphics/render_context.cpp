//
//

#include "render_context.h"
#include "common.h"

using namespace pixel;


RenderContext::RenderContext(glm::ivec2 size, glm::vec4 color)
    : window_size(size),
      default_clear_color(color)
{
}


void RenderContext::update_viewport() const
{
    glViewport(0, 0, window_size.x, window_size.y);
}


glm::mat4 RenderContext::projection() const
{
    return glm::ortho(0.0f, (float) window_size.x, 0.0f, (float) window_size.y);
}

float RenderContext::base_scale_from_virtual_resolution(const glm::vec2& res)
{
    auto frame = glm::vec2(window_size);

    if (frame.y / frame.x > res.y / res.x) {
        return frame.x / res.x;
    } else {
        return frame.y / res.y;
    }
}

void RenderContext::set_base_scale_from_virtual_resolution(const glm::vec2& res)
{
    base_scale = base_scale_from_virtual_resolution(res);
}
