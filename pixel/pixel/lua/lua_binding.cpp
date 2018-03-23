#include <functional>
#include <pixel/pixel.h>
#include <pixel/graphics/common.h>
#include "lua_binding.h"


namespace pixel
{

using namespace std;
using namespace pixel::graphics;
using namespace pixel::input;

struct WindowWrapper
{
    GLFWwindow* window;
};


sol::table bind_pixel(sol::state& lua)
{
    sol::table binding = lua["pixel"] = lua.create_table();

    bind_opengl(lua, binding);
    bind_glm(lua, binding);
    bind_app(lua, binding);
    bind_tile_map(lua, binding);
    bind_tileset(lua, binding);
    bind_tile_layer(lua, binding);
    bind_tile_map_renderer(lua, binding);
    bind_sprite_renderer(lua, binding);
    bind_camera(lua, binding);
    bind_texture_atlas(lua, binding);
    bind_sprite_animation(lua, binding);
    bind_sprite(lua, binding);
    bind_sprite_batch(lua, binding);
    bind_texture_region(lua, binding);
    bind_keyboard(lua, binding);
    bind_image_data(lua, binding);
    bind_offscreen_render_target(lua, binding);

    return binding;
}


void bind_app(sol::state& lua, sol::table& binding, const string& type_name)
{

    auto create = [](sol::table args) -> unique_ptr<App> {
        int w = args["width"];
        int h = args["height"];

        sol::table color = args["background_color"];
        auto bg_color = glm::vec4(color[1], color[2], color[3], color[4]);

        float pixel_scale = args["pixel_scale"];

        if (pixel_scale == 0.0) pixel_scale = 1.0;

        cout << "w = " << w << " h = " << h << " color = " << bg_color.x << " pixel_scale = " << pixel_scale << endl;

        return std::make_unique<App>(glm::ivec2{w, h}, bg_color, pixel_scale);
    };

    binding.new_usertype<App>(
        type_name,
        sol::constructors<App(), App(glm::ivec2, glm::vec4, float)>(),
        "init", &App::init,
        "run", &App::run,
        "set_tick_callback", &App::set_tick_callback,
        "render_context", &App::render_context,
        "window", [](App* self) {
            WindowWrapper w;
            w.window = self->window();
            return w;
        },
        "create", create
    );
}

void bind_tile_map(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<TileMap>(
        type_name,
        "new", sol::constructors<TileMap()>(),
        "load", sol::resolve<const std::string&>(&TileMap::load),
        "update", &TileMap::update,
        "layers", &TileMap::layers,
        "tileset", &TileMap::tileset,
        "tile_count", &TileMap::tile_count,
        "tile_size", &TileMap::tile_size
    );
}

void bind_tile_layer(sol::state& lua, sol::table& binding, const string& type_name)
{
    sol::table tile_layer_binding = binding.new_usertype<TileLayer>(
        type_name,
        "new", sol::constructors<TileLayer(), TileLayer(unsigned width, unsigned height), TileLayer(const TileLayer&)>(),
        "update", &TileLayer::update,
        "tiles", &TileLayer::tiles,
        "animations", &TileLayer::animations,
        "width", &TileLayer::width,
        "height", &TileLayer::height,
        "parallax", &TileLayer::parallax,
        "at", sol::resolve<TileLayer::Tile&(unsigned, unsigned)>(&TileLayer::at)
    );

    tile_layer_binding.new_usertype<TileLayer::Tile>(
        "Tile",
        "new", sol::constructors<TileLayer::Tile(), TileLayer(uint32_t, uint8_t)>(),
        "tile_id", &TileLayer::Tile::tile_id,
        "flip_flags", &TileLayer::Tile::flip_flags
    );
}

void bind_tile_map_renderer(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<TileMapRenderer>(
        type_name,
        "new", sol::constructors<TileMapRenderer()>(),
        "render", &TileMapRenderer::render
    );
}

void bind_sprite_renderer(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<graphics::SpriteRenderer>(
        type_name,
        "new", sol::constructors<graphics::SpriteRenderer()>(),
        "render", &graphics::SpriteRenderer::render
    );
}

void bind_texture_atlas(sol::state& lua, sol::table& binding, const string& type_name)
{
    using TA = graphics::TextureAtlas;

    binding.new_usertype<TA>(
        type_name,

        sol::constructors<TA(), TA(const glm::uvec3&)>(),

        "start_batch", &TA::start_batch,
        "stop_batch", &TA::stop_batch,

        "add_image", sol::overload(
            sol::resolve<uint32_t(const string&)>(&TA::add_image),
            sol::resolve<uint32_t(const string&, const string&)>(&TA::add_image),
            sol::resolve<uint32_t(const ImageData&, const string&)>(&TA::add_image)
        ),

        "lookup", sol::overload(
            sol::resolve<graphics::TextureRegion(uint32_t)const>(&TA::lookup),
            sol::resolve<graphics::TextureRegion(const string&)const>(&TA::lookup)
        ),

        "debug_print", &TA::debug_print,
        "as_texture", sol::resolve<graphics::Texture() const>(&TA::as_texture),
        "layers", &TA::layers
    );
}

void bind_sprite_animation(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<SpriteAnimation>(
        type_name,
        sol::constructors<SpriteAnimation()>(),
        "add_frame", sol::resolve<void(const TextureRegion&, float)>(&SpriteAnimation::add_frame),
        "update", &SpriteAnimation::update,
        "update_sprite", &SpriteAnimation::update_sprite,
        "bool", &SpriteAnimation::validate,
        "current_frame", &SpriteAnimation::current_frame,
        "reset", &SpriteAnimation::reset,
        "time_scale", &SpriteAnimation::time_scale,
        "set_time_scale", &SpriteAnimation::set_time_scale,
        "advance", &SpriteAnimation::advance,
        "copy", &SpriteAnimation::copy
    );
};

void bind_sprite(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<Sprite>(
        type_name,
        sol::constructors<Sprite()>(),
        "position", &Sprite::position,
        "center", &Sprite::center,
        "x", &Sprite::x,
        "y", &Sprite::y,
        "z", &Sprite::z,
        "angle", &Sprite::angle,
        "texture_region", &Sprite::texture_region,

        "flip_h", sol::overload(
            sol::resolve<bool(void)>(&Sprite::flip_h),
            sol::resolve<void(bool)>(&Sprite::flip_h)
        ),

        "flip_v", sol::overload(
            sol::resolve<bool(void)>(&Sprite::flip_v),
            sol::resolve<void(bool)>(&Sprite::flip_v)
        ),

        "flip_d", sol::overload(
            sol::resolve<bool(void)>(&Sprite::flip_d),
            sol::resolve<void(bool)>(&Sprite::flip_d)
        )
    );
}

void bind_sprite_batch(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<SpriteBatch>(
        type_name,
        sol::constructors<SpriteBatch()>(),
        "add", &SpriteBatch::add,
        "restart", &SpriteBatch::restart,
        "sprites", &SpriteBatch::sprites
    );
}

void bind_texture_region(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<TextureRegion>(
        type_name,
        sol::constructors<TextureRegion()>(),
        "x", &TextureRegion::x,
        "y", &TextureRegion::y,
        "w", &TextureRegion::w,
        "h", &TextureRegion::h
    );
}


void bind_keyboard(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<Keyboard>(
        type_name,
        sol::constructors<Keyboard()>(),
        "register_callback", sol::overload(
            &Keyboard::register_callback,

            [](const WindowWrapper& w) {
                Keyboard::register_callback(w.window);
            }
        ),
        "keymap", sol::var(&Keyboard::keymap)
    );
}

void bind_offscreen_render_target(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<OffscreenRenderTarget>(
        type_name,
        sol::constructors<OffscreenRenderTarget()>(),
        "activate", &OffscreenRenderTarget::activate,
        "deactivate", &OffscreenRenderTarget::deactivate,
        "draw", &OffscreenRenderTarget::draw,
        "set_window_size", sol::resolve<void(int,int)>(&OffscreenRenderTarget::set_window_size),
        "window_size", &OffscreenRenderTarget::window_size
    );
}

};
