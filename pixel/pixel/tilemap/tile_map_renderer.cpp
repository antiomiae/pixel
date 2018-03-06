//
//

#include "tile_map_renderer.h"

using namespace pixel;


pixel::TileMapRenderer::TileMapRenderer()
    : buffer_(GL_STATIC_DRAW)
{
    program_ = make_unique<Shader>("assets/shaders/tilemap.vert", "assets/shaders/tilemap.frag");
    tile_layer_texture_ = make_unique<TileLayerTexture>();
    init();
}


void pixel::TileMapRenderer::set_program(pixel::Shader&& p)
{
    program_ = make_unique<Shader>(p);
    init();
}


void pixel::TileMapRenderer::init()
{
    program_->activate();
    vao_.activate();

    buffer_.bindToProgramAttribute(
        *program_,
        "position",
        sizeof(float) * 4,
        sizeof(float) * 0
    );

    buffer_.bindToProgramAttribute(
        *program_,
        "tex_coord",
        sizeof(float) * 4,
        sizeof(float) * 2
    );

    vao_.deactivate();
    program_->deactivate();
}


void TileMapRenderer::set_buffer_data(float map_width, float map_height, float table_width, float table_height)
{
    float data[] = {
        0, 0, 0, table_height,
        0, map_height, 0, 0,
        map_width, 0, table_width, table_height,
        map_width, map_height, table_width, 0
    };

    buffer_.loadData(data, sizeof(data));
}


void pixel::TileMapRenderer::render(pixel::TileMap& t, Camera& camera)
{
    auto map_size = t.tile_size() * t.tile_count();
    auto table_size = t.tile_count();

    auto projection = camera.projection_matrix() * camera.view_matrix();

    set_buffer_data(map_size.x, map_size.y, table_size.x, table_size.y);

    program_->activate();
    vao_.activate();

    program_->setUniform("projection", projection);
    program_->setUniform("tile_size", t.tile_size());

    /* Bind atlas texture to unit 0 */
    t.atlas().activate_texture(0);
    /* Bind map texture to unit 1 */


    program_->setUniform("atlas_tex", 0);
    program_->setUniform("map_tex", 1);

    for (auto& layer : t.layers()) {
        tile_layer_texture_->load(layer, t.atlas());
        tile_layer_texture_->texture().activate(1);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
