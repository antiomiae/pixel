#include <pixel/pixel.h>
#include "lua_binding.h"


namespace pixel
{

using namespace std;
using namespace pixel::graphics;


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


}

