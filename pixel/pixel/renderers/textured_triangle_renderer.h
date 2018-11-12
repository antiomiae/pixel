#ifndef PIXEL_MAIN_TEXTURED_TRIANGLE_RENDERER_H
#define PIXEL_MAIN_TEXTURED_TRIANGLE_RENDERER_H

#include <pixel/math.h>
#include <pixel/graphics/common.h>
#include "renderer_group.h"

namespace pixel::renderers
{

using namespace pixel::graphics;

class TexturedTriangleRenderer : public Renderer
{
public:

    struct Vertex
    {
        glm::vec3 position{};
        glm::vec3 tex_coord{};
        glm::vec4 color{};
    };

    struct Triangle
    {
        Vertex vertices[3] = {};
    };

    TexturedTriangleRenderer();

    void render(const vector<Triangle>& tris, Texture& atlas_texture, Camera& camera);

private:
    void init();
    Vao vao_;
    Buffer vertex_buffer_;
    Shader program_;

};

}

#endif //PIXEL_MAIN_TEXTURED_TRIANGLE_RENDERER_H