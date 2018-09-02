#include <pixel/objects/test_game_object.h>
#include <test/setup.h>

namespace
{

using namespace pixel;

TEST_CASE("GameObject")
{
    GameObject object{};

    REQUIRE_NOTHROW(object.children());
}

};


