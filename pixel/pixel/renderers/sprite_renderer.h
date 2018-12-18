#ifndef PIXEL_SPRITE_RENDERER_H
#define PIXEL_SPRITE_RENDERER_H

#include <vector>
#include "renderer_group.h"
#include <pixel/graphics/graphics.h>
#include <pixel/graphics/sprite.h>

namespace pixel::renderers
{

using namespace std;
using namespace graphics;

class SpriteRenderer : public Renderer
{

public:
    SpriteRenderer();
    Shader& program();
    void render(const vector<pixel::graphics::Sprite>& sprites, Texture& atlas_texture, Camera& camera);

private:
    Vao vao_;
    Buffer sprite_buffer_;
    Buffer vertex_buffer_;
    IndexBuffer<GLubyte> index_buffer_;
    Shader program_;

    void init();
    void initVertexBuffer();
    void initIndexBuffer();
    void bindAttributes();
};

};
#endif //PIXEL_SPRITE_RENDERER_H
