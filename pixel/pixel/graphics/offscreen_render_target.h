

#ifndef PIXEL_OFFSCREEN_RENDERING_H
#define PIXEL_OFFSCREEN_RENDERING_H

#include <pixel/math/math.h>

namespace pixel::graphics
{

class OffscreenRenderTarget
{
public:
    OffscreenRenderTarget();

    void activate();

    void deactivate();

    void draw(const glm::vec4& draw_region);

private:
    unsigned fbo_;
    unsigned rbo_color_;
    unsigned rbo_depth_stencil_;
    glm::ivec2 size_;

    void resize_buffers_(unsigned width, unsigned height);
    void resize_buffers(unsigned int width, unsigned int height);
    void attach_buffers();
};

};

#endif //PIXEL_OFFSCREEN_RENDERING_H
