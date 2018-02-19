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
    Vao vao_;
    Buffer sprite_buffer_;
    Buffer vertex_buffer_;
    IndexBuffer <GLubyte> index_buferr_;
    Shader program_;

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
