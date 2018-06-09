#ifndef PIXEL_LINE_RENDERER_H
#define PIXEL_LINE_RENDERER_H

#include <vector>
#include <pixel/graphics/graphics.h>
#include <pixel/types.h>

namespace pixel::graphics::renderers
{

using namespace std;
using namespace graphics;

class LineRenderer
{

public:

    LineRenderer();

    void render(vector<LineSegment> const&, Camera const&);
    void render(vector<LineSegment> const&, glm::vec4 const& color, Camera const&);
    void render(vector<pair<LineSegment, glm::vec4>> const&, Camera const&);

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
