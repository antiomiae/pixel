//
//

#include "tile_map_renderer.h"

using namespace pixel;


pixel::TileMapRenderer::TileMapRenderer()
    : _buffer(GL_STATIC_DRAW)
{

}


pixel::TileMapRenderer::TileMapRenderer(pixel::Shader&& p)
    : TileMapRenderer{}
{
    _program = make_unique<Shader>(p);
    init();
}


void pixel::TileMapRenderer::setProgram(pixel::Shader&& p)
{
    _program = make_unique<Shader>(p);
}


void pixel::TileMapRenderer::init()
{
    _program->activate();
    _vao.activate();

    _buffer.bindToProgramAttribute(
        *_program,
        "position",
        sizeof(float) * 4,
        sizeof(float) * 0
    );

    _buffer.bindToProgramAttribute(
        *_program,
        "tex_coord",
        sizeof(float) * 4,
        sizeof(float) * 2
    );

    _vao.deactivate();
    _program->deactivate();
}


void TileMapRenderer::setBufferData(float w, float h, float tw, float th)
{
    float data[] = {
        0, 0, 0, 0,
        0, h, 0, th,
        w, 0, tw, 0,
        w, h, tw, th
    };

    _buffer.loadData(data, sizeof(data));
}


void pixel::TileMapRenderer::render(pixel::TileMap& t)
{
    auto quad_size = t.tileSize() * t.tileCount();
    auto map_size = t.tileCount();

    setBufferData(quad_size.x, quad_size.y, map_size.x, map_size.y);
}
