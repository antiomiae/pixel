

#ifndef PIXEL_MAIN_TEXTURED_TRIANGLE_RENDERER_H
#define PIXEL_MAIN_TEXTURED_TRIANGLE_RENDERER_H

#include <pixel/math.h>

namespace pixel::renderers
{

class TexturedTriangleRenderer
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 tex_coord;
        glm::vec4 color;
    };

    struct Triangle
    {
        Vertex vertices[3];
    };

};

}

#endif //PIXEL_MAIN_TEXTURED_TRIANGLE_RENDERER_H
