//
//

#include "sprite_renderer.h"

struct Vertex
{
    GLfloat position[2];
    GLfloat texture_coord[2];
};

pixel::graphics::SpriteRenderer::SpriteRenderer(Shader program)
        : _program(std::move(program)),
          _vertex_buffer(GL_STATIC_DRAW),
          _sprite_buffer(GL_STREAM_DRAW)
{
    init();
}

void pixel::graphics::SpriteRenderer::bindAttributes()
{
    _program.activate();
    _vao.activate();

    /* Bind attributes to buffers */
    _vertex_buffer.bindToProgramAttribute(
            _program,
            "vertex",
            sizeof(Vertex),
            offsetof(Vertex, position)
    );

    _vertex_buffer.bindToProgramAttribute(
            _program,
            "texture_coord",
            sizeof(Vertex),
            offsetof(Vertex, texture_coord)
    );

    _sprite_buffer.bindToProgramAttribute(
            _program,
            "position",
            sizeof(Sprite),
            offsetof(Sprite, position),
            1
    );

    _sprite_buffer.bindToProgramAttribute(
            _program,
            "center",
            sizeof(Sprite),
            offsetof(Sprite, center),
            1
    );

    _sprite_buffer.bindToProgramAttribute(
            _program,
            "angle",
            sizeof(Sprite),
            offsetof(Sprite, angle),
            1
    );

    _sprite_buffer.bindToProgramAttribute(
            _program,
            "texture_region",
            sizeof(Sprite),
            offsetof(Sprite, texture_region) + offsetof(TextureRegion, rect),
            1
    );

    _sprite_buffer.bindToProgramAttribute(
            _program,
            "texture_layer",
            sizeof(Sprite),
            offsetof(Sprite, texture_region) + offsetof(TextureRegion, layer),
            1
    );

    _vao.deactivate();
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

    _vertex_buffer.loadData(vertices, sizeof(vertices));
}

void pixel::graphics::SpriteRenderer::initIndexBuffer() {
    uint8_t indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    _index_buffer.loadData(indices, sizeof(indices));
}

void pixel::graphics::SpriteRenderer::render(pixel::graphics::Sprite *sprites, int count)
{
    _sprite_buffer.loadData(sprites, count * sizeof(Sprite));

    _program.activate();
    _vao.activate();
    _index_buffer.bind();

    glDrawElementsInstanced(GL_TRIANGLES, 6, _index_buffer.elementType(), 0, count);
}
