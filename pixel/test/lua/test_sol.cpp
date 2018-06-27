#include "test/setup.h"
#include <pixel/lua/binding.h>

namespace
{

TEST_CASE("SolTest")
{
    SECTION("bind_pixel") {
        sol::state lua;
        lua.open_libraries();

        auto binding = pixel::binding::bind_pixel(lua);

        binding.set("app", &pixel::app);

        REQUIRE_NOTHROW(
            lua.script("pixel.app()")
        );
    }
}

};

