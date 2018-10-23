#ifndef PIXEL_MAIN_LEVEL_H
#define PIXEL_MAIN_LEVEL_H

#include <pixel/app/app.h>
#include <pixel/graphics/graphics.h>
#include <pixel/renderers/renderers.h>
#include <pixel/math.h>

namespace pixel
{

using namespace graphics;

class Level
{
public:
    Level(glm::ivec2 window_size)
        : window_size_(window_size)
    {
        camera_.set_window_size(window_size);
        render_target_.set_window_size(window_size);
    }

    Camera& camera()
    {
        return camera_;
    }

    void begin_render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        //render_target_.activate();
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void finish_render()
    {
//        render_target_.deactivate();
//
//        /* blit virtual window to actual window */
//        render_target_.draw(
//            glm::ivec4(
//                0,
//                0,
//                pixel::app().render_context().window_size.x,
//                pixel::app().render_context().window_size.y
//            )
//        );
    }

    renderers::RendererGroup& renderer_group()
    {
        return renderer_group_;
    }

    void load_sprites(vector<string> const& paths)
    {
        texture_atlas_.start_batch();
        for_each(begin(paths), end(paths), [&](auto& path) { texture_atlas_.add_image(path); });
        texture_atlas_.stop_batch();

        cout << texture_atlas_.debug_print() << endl;

        sprite_texture_ = texture_atlas_.as_texture();
    }

    Sprite get_sprite(string const& path)
    {
        Sprite s;
        s.texture_region = texture_atlas_.lookup(path);
        return s;
    }

    Texture& sprite_texture()
    {
        return sprite_texture_;
    }

    void load_tile_map(const string& path)
    {
        tile_map_ = TileMap::from_path(path);
    }

    TileMap& tile_map()
    {
        return *tile_map_;
    }

    TextureAtlas& texture_atlas()
    {
        return texture_atlas_;
    }

private:
    glm::ivec2 window_size_;
    Camera camera_;
    renderers::RendererGroup renderer_group_;
    TextureAtlas texture_atlas_;
    Texture sprite_texture_;
    OffscreenRenderTarget render_target_{};
    unique_ptr<TileMap> tile_map_;
};

}


#endif //PIXEL_MAIN_LEVEL_H
