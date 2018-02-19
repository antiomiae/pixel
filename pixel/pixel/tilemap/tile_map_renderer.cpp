//
//

#include "tile_map_renderer.h"

using namespace pixel;


pixel::TileMapRenderer::TileMapRenderer()
    : _buffer(GL_STATIC_DRAW)
{
    _program = make_unique<Shader>("assets/shaders/tilemap.vert", "assets/shaders/tilemap.frag");
    init();
}


void pixel::TileMapRenderer::set_program(pixel::Shader&& p)
{
    _program = make_unique<Shader>(p);
    init();
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


void TileMapRenderer::set_buffer_data(float map_width, float map_height, float table_width, float table_height)
{
    float data[] = {
        0, 0, 0, table_height,
        0, map_height, 0, 0,
        map_width, 0, table_width, table_height,
        map_width, map_height, table_width, 0
    };

    _buffer.loadData(data, sizeof(data));
}


void pixel::TileMapRenderer::render(pixel::TileMap& t, RenderContext render_context)
{
    auto map_size = t.tile_size() * t.tile_count();
    auto table_size = t.tile_count();

    auto projection = render_context.projection() *
                      glm::scale(glm::mat4(), {render_context.base_scale, render_context.base_scale, 1.0});

    set_buffer_data(map_size.x, map_size.y, table_size.x, table_size.y);

    _program->activate();
    _vao.activate();

    _program->setUniform("projection", projection);
    _program->setUniform("tile_size", t.tile_size());

    /* Bind atlas texture to unit 0 */
    t.atlas().activate(0);
    _program->setUniform("atlas_tex", 0);

    for (auto& layer : t.layers()) {
        layer.texture().activate(1);
        _program->setUniform("map_tex", 1);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
