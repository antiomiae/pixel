//
//

#include <pixel/pixel.h>
#include "sprite_renderer.h"

namespace pixel::graphics::renderers
{

struct Vertex
{
    GLfloat position[2];
    GLfloat texture_coord[2];
};

SpriteRenderer::SpriteRenderer()
    : vertex_buffer_(GL_STATIC_DRAW),
      sprite_buffer_(GL_STREAM_DRAW)
{
    program_ = Shader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
    init();
}

void SpriteRenderer::bindAttributes()
{
    program_.activate();
    vao_.activate();

    /* Bind attributes to buffers */
    vertex_buffer_.bindToProgramAttribute(
        program_,
        "vertex",
        sizeof(Vertex),
        offsetof(Vertex, position)
    );

    vertex_buffer_.bindToProgramAttribute(
        program_,
        "texture_coord",
        sizeof(Vertex),
        offsetof(Vertex, texture_coord)
    );

    sprite_buffer_.bindToProgramAttribute(
        program_,
        "position",
        sizeof(Sprite),
        offsetof(Sprite, position),
        1
    );

    sprite_buffer_.bindToProgramAttribute(
        program_,
        "center",
        sizeof(Sprite),
        offsetof(Sprite, center),
        1
    );

    sprite_buffer_.bindToProgramAttribute(
        program_,
        "angle",
        sizeof(Sprite),
        offsetof(Sprite, angle),
        1
    );

    sprite_buffer_.bindToProgramAttribute(
        program_,
        "texture_region",
        sizeof(Sprite),
        offsetof(Sprite, texture_region) + offsetof(TextureRegion, rect),
        1
    );

    sprite_buffer_.bindToProgramAttribute(
        program_,
        "texture_layer",
        sizeof(Sprite),
        offsetof(Sprite, texture_region) + offsetof(TextureRegion, layer),
        1
    );

    sprite_buffer_.bindToProgramAttribute(
        program_,
        "flipped",
        sizeof(Sprite),
        offsetof(Sprite, texture_region) + offsetof(TextureRegion, flipped),
        1
    );

    // flip_flags
    sprite_buffer_.bindToProgramAttribute(
        program_,
        "flip_flags",
        sizeof(Sprite),
        offsetof(Sprite, flip_flags),
        1
    );

    // color
    sprite_buffer_.bindToProgramAttribute(
        program_,
        "color",
        sizeof(Sprite),
        offsetof(Sprite, color),
        1
    );

    // tint
    sprite_buffer_.bindToProgramAttribute(
        program_,
        "tint",
        sizeof(Sprite),
        offsetof(Sprite, tint),
        1
    );

    vao_.deactivate();
}

void SpriteRenderer::init()
{
    bindAttributes();
    initVertexBuffer();
    initIndexBuffer();
}

void SpriteRenderer::initVertexBuffer()
{
    Vertex vertices[] = {
        {0, 0, 0, 1},
        {1, 0, 1, 1},
        {1, 1, 1, 0},
        {0, 1, 0, 0}
    };

    vertex_buffer_.loadData(vertices, sizeof(vertices));
}

void SpriteRenderer::initIndexBuffer()
{
    uint8_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    index_buffer_.loadData(indices, sizeof(indices));
}

void SpriteRenderer::render(const vector<Sprite>& sprites, Texture& atlas_texture, Camera& camera)
{
    auto projection = camera.projection_matrix();
    auto view = camera.view_matrix();
    auto mat = projection * view;

    sprite_buffer_.loadData(sprites.data(), sprites.size() * sizeof(Sprite));
    log_gl_errors();

    program_.activate();
    log_gl_errors();

    vao_.activate();
    log_gl_errors();

    atlas_texture.activate(0);
    log_gl_errors();

    index_buffer_.bind();
    log_gl_errors();

    program_.setUniform("projection", projection * view);
    log_gl_errors();

    program_.setUniform("tex", 0);
    log_gl_errors();

    glDrawElementsInstanced(GL_TRIANGLES, 6, index_buffer_.elementType(), 0, sprites.size());
    log_gl_errors();

    vao_.deactivate();
    log_gl_errors();

    program_.deactivate();
    log_gl_errors();
}

Shader& SpriteRenderer::program()
{
    return program_;
}

};
