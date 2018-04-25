

#ifndef PIXEL_OFFSCREEN_RENDERING_H
#define PIXEL_OFFSCREEN_RENDERING_H

#include <pixel/math.h>

namespace pixel::graphics
{

class OffscreenRenderTarget
{
public:
    OffscreenRenderTarget();

    void activate();
    void deactivate();
    void draw(const glm::ivec4& draw_region);
    glm::ivec2 window_size() const;
    void set_window_size(const glm::ivec2&);
    void set_window_size(int, int);

private:
    unsigned fbo_;
    unsigned rbo_color_;
    unsigned rbo_depth_stencil_;
    glm::ivec2 size_;
    glm::ivec4 old_viewport_;

    void resize_buffers(unsigned int width, unsigned int height);
    void attach_buffers();
};

};

#endif //PIXEL_OFFSCREEN_RENDERING_H
