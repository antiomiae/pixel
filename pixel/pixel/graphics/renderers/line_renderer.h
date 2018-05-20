#ifndef PIXEL_LINE_RENDERER_H
#define PIXEL_LINE_RENDERER_H

#include <vector>
#include <pixel/graphics/shader.h>
#include <pixel/graphics/camera.h>
#include <pixel/graphics/vao.h>
#include <pixel/graphics/buffer.h>
#include <pixel/types.h>

namespace pixel::graphics::renderers
{

using namespace std;
using namespace graphics;

class LineRenderer
{

public:

    LineRenderer();

    void render(const vector<LineSegment>&, const Camera&);

private:

    Shader program_;
    Vao vao_;
    Buffer position_buffer_;
    Buffer color_buffer_;

    void bind_attributes();
    void init();
};

};

#endif //PIXEL_LINE_RENDERER_H
