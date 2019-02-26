#ifndef PIXEL_MAIN_TRIANGE_BATCH_H
#define PIXEL_MAIN_TRIANGE_BATCH_H

#include <pixel/pixel.h>
#include <vector>

using namespace std;

using Triangle = pixel::renderers::TexturedTriangleRenderer::Triangle;
using Vertex = pixel::renderers::TexturedTriangleRenderer::Vertex;

class TriangleBatch
{
public:
    void add_sprite(const pixel::graphics::Sprite& sprite);

    vector<Triangle>& triangles();

private:
    vector<Triangle> tris_;
};


#endif //PIXEL_MAIN_TRIANGE_BATCH_H
