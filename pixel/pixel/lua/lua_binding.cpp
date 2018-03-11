#include "lua_binding.h"
#include "../pixel.h"

using namespace pixel::graphics;

namespace pixel
{

sol::table bind_pixel(sol::state& lua)
{
    sol::table binding = lua["pixel"] = lua.create_table();

    bind_glm(lua, binding);
    bind_app(lua, binding);
    bind_tile_map(lua, binding);
    bind_tile_map_renderer(lua, binding);
    bind_sprite_renderer(lua, binding);
    bind_camera(lua, binding);
    bind_texture_atlas(lua, binding);
    bind_sprite_animation(lua, binding);
    bind_sprite(lua, binding);
    bind_sprite_batch(lua, binding);
    bind_texture_region(lua, binding);

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
        "create", create
    );
}

void bind_glm(sol::state& lua, sol::table& binding, const string& module_name)
{
    sol::table _glm = binding[module_name] = lua.create_table();

    _glm.new_usertype<glm::vec2>(
        "vec2",
        "new", sol::constructors<glm::vec2(), glm::vec2(float, float), glm::vec2(glm::ivec2)>()
    );

    _glm.new_usertype<glm::ivec2>(
        "ivec2",
        "new", sol::constructors<glm::ivec2(), glm::ivec2(int, int)>()
    );

    _glm.new_usertype<glm::uvec2>(
        "uvec2",
        "new", sol::constructors<glm::uvec2(), glm::uvec2(unsigned, unsigned)>()
    );

    _glm.new_usertype<glm::vec3>(
        "vec3",
        "new", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>()
    );

    _glm.new_usertype<glm::ivec3>(
        "ivec3",
        "new", sol::constructors<glm::ivec3(), glm::ivec3(int, int, int)>()
    );

    _glm.new_usertype<glm::uvec3>(
        "uvec3",
        "new", sol::constructors<glm::uvec3(), glm::uvec3(unsigned, unsigned, unsigned)>()
    );

    _glm.new_usertype<glm::vec4>(
        "vec4",
        "new", sol::constructors<glm::vec4(void), glm::vec4(float, float, float, float)>()
    );

    _glm.new_usertype<glm::ivec4>(
        "ivec4",
        "new", sol::constructors<glm::ivec4(), glm::ivec4(int, int, int, int)>()
    );

    _glm.new_usertype<glm::uvec4>(
        "uvec4",
        "new", sol::constructors<glm::uvec4(), glm::uvec4(unsigned, unsigned, unsigned, unsigned)>()
    );

    _glm.new_usertype<glm::mat2>(
        "mat2",
        "new", sol::constructors<glm::mat2(), glm::mat2(float, float,
                                                        float, float)>()
    );

    _glm.new_usertype<glm::mat3>(
        "mat3",
        "new", sol::constructors<glm::mat3(), glm::mat3(float, float, float,
                                                        float, float, float,
                                                        float, float, float)>()
    );

    _glm.new_usertype<glm::mat4>(
        "mat4",
        "new", sol::constructors<glm::mat4(), glm::mat4(float, float, float, float,
                                                        float, float, float, float,
                                                        float, float, float, float,
                                                        float, float, float, float)>()
    );
}



void bind_tile_map(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<TileMap>(
        type_name,
        "new", sol::constructors<TileMap()>(),
        "load", sol::resolve<const std::string&>(&TileMap::load),
        "update", &TileMap::update
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

void bind_camera(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<graphics::Camera>(
        type_name,

        "new", sol::constructors<graphics::Camera(), graphics::Camera(glm::ivec2, glm::vec4)>(),

        "lock_x", &graphics::Camera::lock_x,
        "lock_y", &graphics::Camera::lock_y,

        "translate", sol::overload(
            sol::resolve<void(float, float)>(&graphics::Camera::translate),
            sol::resolve<void(const glm::vec2&)>(&graphics::Camera::translate)
        ),

        "center_at", sol::overload(
            sol::resolve<void(float, float)>(&graphics::Camera::center_at),
            sol::resolve<void(const glm::vec2&)>(&graphics::Camera::center_at)
        ),

        "follow", sol::overload(
            sol::resolve<void(float, float)>(&graphics::Camera::follow),
            sol::resolve<void(const glm::vec2&)>(&graphics::Camera::follow)
        ),

        "position_at", sol::overload(
            sol::resolve<void(float, float)>(&graphics::Camera::position_at),
            sol::resolve<void(const glm::vec2&)>(&graphics::Camera::position_at)
        ),

        "scale_by", sol::overload(
            sol::resolve<void(float, float)>(&graphics::Camera::scale_by),
            sol::resolve<void(const glm::vec2&)>(&graphics::Camera::scale_by)
        ),

        "scale", sol::overload(
            sol::resolve<glm::vec2()>(&graphics::Camera::scale),
            sol::resolve<void(float)>(&graphics::Camera::scale),
            sol::resolve<void(float, float)>(&graphics::Camera::scale),
            sol::resolve<void(const glm::vec2&)>(&graphics::Camera::scale)
        ),

        "set_window_size", sol::overload(
            sol::resolve<void(int, int)>(&graphics::Camera::set_window_size),
            sol::resolve<void(const glm::ivec2&)>(&graphics::Camera::set_window_size)
        ),

        "view_matrix", &graphics::Camera::view_matrix,

        "projection_matrix", &graphics::Camera::projection_matrix,

        "position", &graphics::Camera::position,

        "window_size", &graphics::Camera::window_size,

        "view_rect", &graphics::Camera::view_rect
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
            sol::resolve<uint32_t(const string&, const string&)>(&TA::add_image)
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
        "texture_region", &Sprite::texture_region
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

};
