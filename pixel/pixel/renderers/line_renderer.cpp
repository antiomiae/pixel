

#include "line_renderer.h"

namespace pixel::renderers
{

LineRenderer::LineRenderer()
{
    shader_ = Shader("assets/shaders/line_renderer.vert", "assets/shaders/line_renderer.frag");
}

void LineRenderer::render(const vector<LineSegment>& line_segments, const Camera& camera)
{

}

}
