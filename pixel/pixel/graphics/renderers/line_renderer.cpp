

#include <pixel/pixel.h>
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
    position_buffer_.load_data(line_segments);

    vector<glm::vec4> colors{line_segments.size() * 2, glm::vec4{1.0, 1.0, 1.0, 1.0}};
    color_buffer_.load_data(colors);

    auto projection = camera.projection_matrix();
    auto view = camera.view_matrix();
    auto mat = projection * view;

    program_.activate();
    log_gl_errors();

    vao_.activate();
    log_gl_errors();

    program_.setUniform("projection", projection * view);
    log_gl_errors();

    glDrawArrays(GL_LINES, 0, line_segments.size() * 2);
    log_gl_errors();

    vao_.deactivate();
    log_gl_errors();

    program_.deactivate();
    log_gl_errors();
}

}
