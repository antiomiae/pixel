#ifndef PIXEL_RENDER_CONTEXT_H
#define PIXEL_RENDER_CONTEXT_H

#include "../math/math.h"

namespace pixel
{
/**
 * Information about the global rendering setup
 */
struct RenderContext
{
    /**
     * Dimensions of drawing area
     */
    glm::ivec2 window_size{0, 0};

    /**
     * Number of screen pixels per internal pixel
     */
    float pixel_scale{1.0};

    glm::vec4 default_clear_color{0.0, 0.0, 0.0, 1.0};

    RenderContext() = default;
    RenderContext(glm::ivec2 size, glm::vec4 color, float scale);

    /**
     * Orthographic projection matrix derived from `window_size` and `pixel_scale`
     * Transforms the region [(0, 0), (window_size / pixel_scale)] to [(-1, -1), (1, 1)]
     * @return projection matrix
     */
    glm::mat4 projection() const;

};

};

#endif //PIXEL_RENDER_CONTEXT_H
