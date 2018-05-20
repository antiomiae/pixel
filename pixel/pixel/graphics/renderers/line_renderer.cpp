

#include "line_renderer.h"

namespace pixel::graphics::renderers
{

LineRenderer::LineRenderer()
    : position_buffer_{GL_STREAM_DRAW},
      color_buffer_{GL_STREAM_DRAW}
{
    init();
}

void LineRenderer::bind_attributes()
{
    program_.activate();
    vao_.activate();

    /* Bind attributes to buffers */
    position_buffer_.bindToProgramAttribute(
        program_,
        "position",
        0, /* tightly packed */
        0
    );

    color_buffer_.bindToProgramAttribute(
        program_,
        "color",
        0,
        0
    );

    vao_.deactivate();
}

void LineRenderer::init()
{
    program_ = Shader("assets/shaders/line_renderer.vert", "assets/shaders/line_renderer.frag");
    bind_attributes();
}


void LineRenderer::render(const vector<LineSegment>& line_segments, const Camera& camera)
{

}

}
