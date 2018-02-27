//
//

#ifndef PIXEL_SPRITE_RENDERER_H
#define PIXEL_SPRITE_RENDERER_H

#include "graphics.h"

namespace pixel::graphics
{

class SpriteRenderer
{
public:
    SpriteRenderer();
    void initVertexBuffer();
    void initIndexBuffer();
    void bindAttributes();
    Shader& program();
    void render(Sprite* sprites, int count);

private:
    Vao vao_;
    Buffer sprite_buffer_;
    Buffer vertex_buffer_;
    IndexBuffer <GLubyte> index_buffer_;
    Shader program_;

    void init();
};

};
#endif //PIXEL_SPRITE_RENDERER_H
