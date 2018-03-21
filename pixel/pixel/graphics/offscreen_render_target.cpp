
#include <pixel/error.h>
#include "graphics.h"
#include "offscreen_render_target.h"

namespace pixel::graphics
{

OffscreenRenderTarget::OffscreenRenderTarget()
{
    glGenFramebuffers(1, &fbo_);
    glGenRenderbuffers(1, &rbo_color_);
    glGenRenderbuffers(1, &rbo_depth_stencil_);

    resize_buffers(320, 240);

    attach_buffers();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    auto is_complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    error_unless(is_complete, "framebuffer incomplete");
}

void OffscreenRenderTarget::activate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, size_.x, size_.y);
}

void OffscreenRenderTarget::deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OffscreenRenderTarget::draw(const glm::vec4& draw_region)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(
        0, 0,
        size_.x, size_.y,
        draw_region.x, draw_region.y,
        draw_region.z, draw_region.w,
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
    );
}

void OffscreenRenderTarget::resize_buffers(unsigned width, unsigned height)
{
    size_ = {width, height};
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_color_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth_stencil_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

}

void OffscreenRenderTarget::attach_buffers()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo_color_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_depth_stencil_);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

};
