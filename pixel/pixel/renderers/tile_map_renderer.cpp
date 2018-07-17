#include "tile_map_renderer.h"
#include <pixel/error.h>


namespace pixel::renderers
{


TileMapRenderer::TileMapRenderer()
    : buffer_(GL_STATIC_DRAW)
{
    program_ = Shader("assets/shaders/tilemap.vert", "assets/shaders/tilemap.frag");
    tile_layer_texture_ = make_unique<TileLayerTexture>();
    init();
}


void TileMapRenderer::init()
{
    program_.activate();
    vao_.activate();

    buffer_.bindToProgramAttribute(
        program_,
        "position",
        sizeof(float) * 4,
        sizeof(float) * 0
    );

    buffer_.bindToProgramAttribute(
        program_,
        "tex_coord",
        sizeof(float) * 4,
        sizeof(float) * 2
    );

    vao_.deactivate();
    program_.deactivate();
}


void TileMapRenderer::set_buffer_data(float map_width, float map_height, float table_width, float table_height)
{
    float data[] = {
        0, 0, 0, 0,
        0, map_height, 0, table_height,
        map_width, 0, table_width, 0,
        map_width, map_height, table_width, table_height
    };

    buffer_.loadData(data, sizeof(data));
}


void TileMapRenderer::render(TileMap& t, Camera& camera)
{
    auto map_size = t.tile_size() * t.tile_count();
    auto table_size = t.tile_count();

    set_buffer_data(map_size.x, map_size.y, table_size.x, table_size.y);
    log_gl_errors();
    program_.activate();
    log_gl_errors();
    vao_.activate();
    log_gl_errors();

    program_.setUniform("tile_size", glm::ivec2(t.tile_size()));
    log_gl_errors();

    //program_.setUniform("tile_count", glm::ivec2(t.tile_count()));
    log_gl_errors();

    /* Bind atlas texture to unit 0 */
    t.atlas().activate_texture(0);
    log_gl_errors();
    /* Bind map texture to unit 1 */

    program_.setUniform("atlas_tex", 0);
    log_gl_errors();

    for (auto& layer : t.layers()) {
        auto p = layer.parallax();

        auto projection = camera.projection_matrix() * camera.parallax_view_matrix(layer.parallax());
        program_.setUniform("projection", projection);
        log_gl_errors();

        tile_layer_texture_->load(layer, t.atlas());
        log_gl_errors();

        tile_layer_texture_->texture().activate(1);
        log_gl_errors();

        program_.setUniform("map_tex", 1);
        log_gl_errors();

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        log_gl_errors();
    }

    vao_.deactivate();
    log_gl_errors();

    program_.deactivate();
    log_gl_errors();
}

};
