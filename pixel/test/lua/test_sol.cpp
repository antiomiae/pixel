#include "test/setup.h"
#include <pixel/lua/binding.h>

namespace
{

TEST(SolTest, State)
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);
}


TEST(SolTest, bind_pixel)
{
    sol::state lua;
    lua.open_libraries();

    auto binding = pixel::binding::bind_pixel(lua);

    binding.set("app", &pixel::app);

    ASSERT_NO_THROW(
        lua.script("pixel.app()")
    );
}


};

