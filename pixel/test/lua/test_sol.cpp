#include "test/setup.h"
#include <pixel/lua/lua.h>

namespace
{

TEST(SolTest, State)
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);
}


TEST(SolTest, Binding)
{
    sol::state lua;
    lua.open_libraries();

    auto binding = pixel::bind_pixel(lua);

    binding.set("app", &pixeltest::app());

    lua.script(R"(
        print(pixel.app)
    )");
}

};

