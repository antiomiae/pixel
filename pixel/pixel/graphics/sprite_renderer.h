//
//

#ifndef PIXEL_SPRITE_RENDERER_H
#define PIXEL_SPRITE_RENDERER_H

#include <vector>
#include "graphics.h"
#include "camera.h"


namespace pixel::graphics
{

using namespace std;

class SpriteRenderer
{
public:
    SpriteRenderer();
    void initVertexBuffer();
    void initIndexBuffer();
    void bindAttributes();
    Shader& program();
    void render(const vector<Sprite>& sprites, Texture& atlas_texture, Camera& camera);

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
