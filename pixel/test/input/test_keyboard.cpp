#include <pixel/input/input.h>
#include <test/setup.h>

namespace
{

using namespace pixel::input;

TEST(Keyboard, register_callback)
{
    Keyboard::register_callback(pixel::app().window());
}

}
