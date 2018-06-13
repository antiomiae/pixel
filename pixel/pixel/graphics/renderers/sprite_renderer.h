#ifndef PIXEL_SPRITE_RENDERER_H
#define PIXEL_SPRITE_RENDERER_H

#include <vector>
#include "renderer_group.h"
#include <pixel/graphics/graphics.h>
#include <pixel/graphics/sprite.h>

namespace pixel::graphics::renderers
{

using namespace std;

class SpriteRenderer : public Renderer
{

public:
    SpriteRenderer();
    void initVertexBuffer();
    void initIndexBuffer();
    void bindAttributes();
    Shader& program();
    void render(const vector<pixel::graphics::Sprite>& sprites, Texture& atlas_texture, Camera& camera);

    virtual ~SpriteRenderer() = default;

private:
    Vao vao_;
    Buffer sprite_buffer_;
    Buffer vertex_buffer_;
    IndexBuffer<GLubyte> index_buffer_;
    Shader program_;

    void init();
    
};

};
#endif //PIXEL_SPRITE_RENDERER_H
