//
//

#include <pixel/pixel.h>
#include "sprite_renderer.h"

namespace pixel::graphics
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

void SpriteRenderer::SpriteRenderer::bindAttributes()
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

    vao_.deactivate();
}

void SpriteRenderer::SpriteRenderer::init()
{
    bindAttributes();
    initVertexBuffer();
    initIndexBuffer();
}

void SpriteRenderer::SpriteRenderer::initVertexBuffer()
{
    Vertex vertices[] = {
        {0, 0, 0, 1},
        {1, 0, 1, 1},
        {1, 1, 1, 0},
        {0, 1, 0, 0}
    };

    vertex_buffer_.loadData(vertices, sizeof(vertices));
}

void SpriteRenderer::SpriteRenderer::initIndexBuffer()
{
    uint8_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    index_buffer_.loadData(indices, sizeof(indices));
}

void SpriteRenderer::SpriteRenderer::render(const vector<Sprite>& sprites, Texture& atlas_texture, Camera& camera)
{
    auto projection = camera.projection_matrix();
    auto view = camera.view_matrix();
    auto mat = projection * view;

    sprite_buffer_.loadData(sprites.data(), sprites.size() * sizeof(Sprite));
    logGlErrors();

    program_.activate();
    logGlErrors();

    vao_.activate();
    logGlErrors();

    atlas_texture.activate(0);
    logGlErrors();

    index_buffer_.bind();
    logGlErrors();

    program_.setUniform("projection", projection * view);
    logGlErrors();

    program_.setUniform("tex", 0);
    logGlErrors();

    glDrawElementsInstanced(GL_TRIANGLES, 6, index_buffer_.elementType(), 0, sprites.size());
    logGlErrors();

    vao_.deactivate();
    logGlErrors();

    program_.deactivate();
    logGlErrors();
}

pixel::graphics::Shader& pixel::graphics::SpriteRenderer::program()
{
    return program_;
}

};
