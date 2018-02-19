//
//

#include "sprite_renderer.h"

struct Vertex
{
    GLfloat position[2];
    GLfloat texture_coord[2];
};

pixel::graphics::SpriteRenderer::SpriteRenderer(Shader program)
        : program_(std::move(program)),
          vertex_buffer_(GL_STATIC_DRAW),
          sprite_buffer_(GL_STREAM_DRAW)
{
    init();
}

void pixel::graphics::SpriteRenderer::bindAttributes()
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

    vao_.deactivate();
}

void pixel::graphics::SpriteRenderer::init()
{
    bindAttributes();
    initVertexBuffer();
    initIndexBuffer();
}

void pixel::graphics::SpriteRenderer::initVertexBuffer() {
    Vertex vertices[] = {
            {0, 0, 0, 1},
            {1, 0, 1, 1},
            {1, 1, 1, 0},
            {0, 1, 0, 0}
    };

    vertex_buffer_.loadData(vertices, sizeof(vertices));
}

void pixel::graphics::SpriteRenderer::initIndexBuffer() {
    uint8_t indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    index_buferr_.loadData(indices, sizeof(indices));
}

void pixel::graphics::SpriteRenderer::render(pixel::graphics::Sprite *sprites, int count)
{
    sprite_buffer_.loadData(sprites, count * sizeof(Sprite));

    program_.activate();
    vao_.activate();
    index_buferr_.bind();

    glDrawElementsInstanced(GL_TRIANGLES, 6, index_buferr_.elementType(), 0, count);
}
