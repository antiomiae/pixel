#include "line_renderer.h"
#include <pixel/pixel.h>

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


void LineRenderer::render(vector<LineSegment> const& line_segments, Camera const& camera)
{
    render(line_segments, {1.0, 1.0, 1.0, 1.0}, camera);
}

void LineRenderer::render(vector<LineSegment> const& line_segments, glm::vec4 const& color, Camera const& camera)
{
    position_buffer_.load_data(line_segments);

    vector<glm::vec4> colors{line_segments.size() * 2, color};
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

void LineRenderer::render(vector<pair<LineSegment, glm::vec4>> const& lines_with_colors, Camera const& camera)
{
    vector<LineSegment> line_segments{lines_with_colors.size()};

    transform(
        begin(lines_with_colors),
        end(lines_with_colors),
        begin(line_segments),
        [](auto& p) { return p.first; }
    );

    position_buffer_.load_data(line_segments);

    vector<glm::vec4> colors{line_segments.size() * 2};

    auto it = begin(colors);

    for (auto const& pair : lines_with_colors) {
        *it++ = pair.second;
        *it++ = pair.second;
    }

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
