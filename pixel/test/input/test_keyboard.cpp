#include <pixel/input/input.h>
#include <test/setup.h>

namespace
{

using namespace pixel::input;

TEST_CASE("Keyboard")
{
    REQUIRE_NOTHROW(Keyboard::register_callback(pixel::app().window()));
}

}
