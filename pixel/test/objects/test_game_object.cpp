#include <pixel/objects/game_object.h>
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


