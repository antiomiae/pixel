#include <pixel/pixel.h>
#include "test/setup.h"
namespace
{

using namespace std;
using pixel::TileLayer;

TEST_CASE("TileAnimation")
{
    TileLayer::TileAnimation ta{};

    ta.animation_definition.frames = {
        {1, 2000/60.0},
        {2, 2000/60.0},
        {3, 2000/60.0}
    };

    /* Should start at beginning of sequence */
    REQUIRE(0 == ta.frame);
    REQUIRE(0.0 == ta.timer);
    REQUIRE(1 == ta.current_tile_id());

    ta.update(1/60.0);
    /* Should still be on first frame */
    REQUIRE(1 == ta.current_tile_id());

    ta.update(1/60.0);
    /* Frame should have advanced */
    REQUIRE(2 == ta.current_tile_id());

    ta.update(1/60.0);
    ta.update(1/60.0);
    REQUIRE(3 == ta.current_tile_id());

    /* Should reset to 0 at end of animation */
    ta.update(1/60.0);
    ta.update(1/60.0);
    REQUIRE(1 == ta.current_tile_id());


};
}
