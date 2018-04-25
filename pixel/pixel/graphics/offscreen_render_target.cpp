
#include <pixel/error.h>
#include "graphics.h"

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
    glGetIntegerv(GL_VIEWPORT, &old_viewport_[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, size_.x, size_.y);
}

void OffscreenRenderTarget::deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(
        old_viewport_.x,
        old_viewport_.y,
        old_viewport_.z,
        old_viewport_.w
    );
}

void OffscreenRenderTarget::draw(const glm::ivec4& draw_region)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_);
    log_gl_errors();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    log_gl_errors();
    //glReadBuffer(GL_COLOR_ATTACHMENT0);

    //glDrawBuffer(GL_BACK);

    glBlitFramebuffer(
        0, 0,
        size_.x, size_.y,
        draw_region.x, draw_region.y,
        draw_region.z, draw_region.w,
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
    );
    log_gl_errors();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    log_gl_errors();
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

glm::ivec2 OffscreenRenderTarget::window_size() const
{
    return size_;
}

void OffscreenRenderTarget::set_window_size(const glm::ivec2& v)
{
    resize_buffers((unsigned) v.x, (unsigned) v.y);
}

void OffscreenRenderTarget::set_window_size(int w, int h)
{
    set_window_size({w, h});
}

};
