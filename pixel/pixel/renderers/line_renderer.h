#ifndef PIXEL_LINE_RENDERER_H
#define PIXEL_LINE_RENDERER_H

#include <vector>
#include <pixel/graphics/shader.h>
#include <pixel/graphics/camera.h>
#include <pixel/graphics/vao.h>
#include <pixel/types.h>

namespace pixel::renderers
{

using namespace std;
using namespace graphics;

class LineRenderer
{

public:

    LineRenderer();

    void render(const vector<LineSegment>&, const Camera&);

private:

    Shader shader_;
    Vao vao_;

};

};

#endif //PIXEL_LINE_RENDERER_H
