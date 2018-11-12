#include <pixel/pixel.h>
#include "textured_triangle_renderer.h"

namespace pixel::renderers
{

TexturedTriangleRenderer::TexturedTriangleRenderer()
: vertex_buffer_(GL_STREAM_DRAW)
{
    program_ = Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    init();
}

void TexturedTriangleRenderer::init()
{
    program_.activate();
    vao_.activate();

    vertex_buffer_.bindToProgramAttribute(program_, "position", sizeof(Vertex), offsetof(Vertex, position), 0);
    vertex_buffer_.bindToProgramAttribute(program_, "tex_coord", sizeof(Vertex), offsetof(Vertex, tex_coord), 0);
    vertex_buffer_.bindToProgramAttribute(program_, "color", sizeof(Vertex), offsetof(Vertex, color), 0);

    vao_.deactivate();
}

void TexturedTriangleRenderer::render(
    const vector<TexturedTriangleRenderer::Triangle>& tris, Texture& atlas_texture, Camera& camera
)
{
    vao_.activate();
    program_.activate();

    auto projection = camera.projection_matrix();
    auto view = camera.view_matrix();
    auto mat = projection * view;

    vertex_buffer_.loadData(tris.data(), tris.size() * sizeof(Triangle));
    log_gl_errors();

    program_.activate();
    log_gl_errors();

    vao_.activate();
    log_gl_errors();

    atlas_texture.activate(0);
    log_gl_errors();

    program_.setUniform("projection", projection * view);
    log_gl_errors();

    program_.setUniform("atlas_tex", 0);
    log_gl_errors();

    glDrawArrays(GL_TRIANGLES, 0, tris.size() * 3);
    log_gl_errors();

    vao_.deactivate();
    log_gl_errors();

    program_.deactivate();
    log_gl_errors();
}

}
