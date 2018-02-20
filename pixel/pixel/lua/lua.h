#ifndef PIXEL_LUA_H
#define PIXEL_LUA_H

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>
#include <pixel/pixel.h>

namespace pixel
{

sol::table bind_pixel(sol::state& lua);


sol::table bind_pixel(sol::state& lua)
{
    return lua["pixel"] = lua.create_table();
}

};

#endif //PIXEL_LUA_H
