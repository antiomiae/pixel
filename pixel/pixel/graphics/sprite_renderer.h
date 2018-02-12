//
//

#ifndef PIXEL_SPRITE_RENDERER_H
#define PIXEL_SPRITE_RENDERER_H

#include "graphics.h"

namespace pixel::graphics
{

class SpriteRenderer
{
private:
    Vao _vao;
    Buffer _sprite_buffer;
    Buffer _vertex_buffer;
    IndexBuffer <GLubyte> _index_buffer;
    Shader _program;

    void init();

public:
    SpriteRenderer(Shader spriteShader);
    void initVertexBuffer();
    void initIndexBuffer();
    void bindAttributes();
    void render(Sprite* sprites, int count);
};

};
#endif //PIXEL_SPRITE_RENDERER_H
