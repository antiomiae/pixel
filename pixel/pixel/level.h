#ifndef PIXEL_MAIN_LEVEL_H
#define PIXEL_MAIN_LEVEL_H

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
    }

    Camera& camera()
    {
        return camera_;
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


private:
    glm::ivec2 window_size_;

    Camera camera_;

    renderers::RendererGroup renderer_group_;

    TextureAtlas texture_atlas_;

    Texture sprite_texture_;
};

}


#endif //PIXEL_MAIN_LEVEL_H
