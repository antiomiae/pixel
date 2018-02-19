#ifndef PIXEL_RENDER_CONTEXT_H
#define PIXEL_RENDER_CONTEXT_H

#include "../math/math.h"

namespace pixel
{
// Information about the global rendering setup
struct RenderContext
{
    // assume we draw in our whole window
    glm::ivec2 window_size{0, 0};
    glm::vec4 default_clear_color{0.0, 0.0, 0.0, 1.0};
    float base_scale{1};

    RenderContext() = default;
    RenderContext(glm::ivec2 size, glm::vec4 color);

    float base_scale_from_virtual_resolution(const glm::vec2& res);
    void set_base_scale_from_virtual_resolution(const glm::vec2& res);
    void update_viewport() const;
    glm::mat4 projection() const;

};

};

#endif //PIXEL_RENDER_CONTEXT_H
