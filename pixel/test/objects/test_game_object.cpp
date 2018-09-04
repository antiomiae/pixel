#include <pixel/objects/game_object.h>
#include <test/setup.h>

namespace
{

using namespace pixel;

TEST_CASE("GameObject")
{
    GameObject object{};

    REQUIRE_NOTHROW(object.children());

    object.add_child(make_unique<GameObject>());

    REQUIRE(object.children()[0]->parent() == &object);

    object.update(1/60.0f);
}

};


