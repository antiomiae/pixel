#include "lua.h"

namespace pixel
{

sol::table bind_pixel(sol::state& lua)
{
    sol::table binding = lua["pixel"] = lua.create_table();

    bind_glm(lua, binding);
    bind_app(lua, binding);
    bind_tile_map(lua, binding);
    bind_tile_map_renderer(lua, binding);


    return binding;
}


void bind_app(sol::state& lua, sol::table& binding, const string& type_name)
{

    auto app_from_table = [](sol::table args) -> unique_ptr<App> {
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
        "new", sol::constructors<App(), App(glm::ivec2, glm::vec4, float)>(),
        "init", &App::init,
        "run", &App::run,
        "set_tick_callback", &App::set_tick_callback,
        "render_context", &App::render_context,
        "create", app_from_table
    );
}


void bind_glm(sol::state& lua, sol::table& binding, const string& module_name);


void bind_tile_map(sol::state& lua, sol::table& binding, const string& type_name)
{
    binding.new_usertype<TileMap>(
        type_name,
        "new", sol::constructors<TileMap()>(),
        "load", sol::resolve<const std::string&>(&TileMap::load),
        "update", &TileMap::update
    );
}


void bind_glm(sol::state& lua, sol::table& binding, const string& module_name)
{
    sol::table _glm = binding[module_name] = lua.create_table();

    _glm.new_usertype<glm::ivec2>(
        "ivec2",
        "new", sol::constructors<glm::ivec2(), glm::ivec2(int, int)>()
    );

    _glm.new_usertype<glm::vec4>(
        "vec4",
        "new", sol::constructors<glm::vec4(void), glm::vec4(float, float, float, float)>()
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


};
