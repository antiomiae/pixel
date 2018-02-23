#ifndef PIXEL_LUA_H
#define PIXEL_LUA_H

#define SOL_CHECK_ARGUMENTS 1

#include <sol.hpp>
#include <pixel/pixel.h>
#include <memory>

namespace pixel
{


sol::table bind_pixel(sol::state& lua);
void bind_app(sol::table& lua, string type_name = "App");


sol::table bind_pixel(sol::state& lua)
{
    sol::table binding = lua["pixel"] = lua.create_table();

    bind_app(binding);

    return binding;
}


void bind_app(sol::table& lua, string type_name)
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

    auto type = lua.new_usertype<App>(
        type_name,
        "new", sol::constructors<App(), App(glm::ivec2, glm::vec4, float)>(),
        "init", &App::init,
        "run", &App::run,
        "set_tick_callback", &App::set_tick_callback,
        "render_context", &App::render_context,
        "create", app_from_table
    );
}

};

#endif //PIXEL_LUA_H
