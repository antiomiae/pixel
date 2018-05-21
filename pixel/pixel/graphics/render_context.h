#ifndef PIXEL_RENDER_CONTEXT_H
#define PIXEL_RENDER_CONTEXT_H

#include "pixel/math.h"

namespace pixel
{
/**
 * Information about the global rendering setup
 */
struct RenderContext
{
    /**
     * Dimensions of window
     */
    glm::ivec2 window_size{0, 0};

    /**
      * Dimensions of virtual drawing area
      */
    glm::ivec2 virtual_window_size{0, 0};

    glm::vec4 default_clear_color{0.0, 0.0, 0.0, 1.0};

    RenderContext() = default;
    RenderContext(const glm::ivec2& real_size, const glm::ivec2& virtual_size, const glm::vec4& color);

    /**
     * Orthographic projection matrix derived from `window_size` and `pixel_scale`
     * Transforms the region [(0, 0), (window_size / pixel_scale)] to [(-1, -1), (1, 1)]
     * @return projection matrix
     */
    glm::mat4 projection() const;
};

};

#endif //PIXEL_RENDER_CONTEXT_H
