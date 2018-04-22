#include <pixel/pixel.h>
#include "test/setup.h"
namespace
{
TEST(TileAnimation, Animation)
{
    pixel::TileLayer::TileAnimation ta{};

    ta.animation_definition.frames = {
        {1, 2000/60.0},
        {2, 2000/60.0},
        {3, 2000/60.0}
    };

    /* Should start at beginning of sequence */
    ASSERT_EQ(0, ta.frame);
    ASSERT_EQ(0.0, ta.timer);
    ASSERT_EQ(1, ta.current_tile_id()) << to_string(ta.timer);

    ta.update(1/60.0);
    /* Should still be on first frame */
    ASSERT_EQ(1, ta.current_tile_id()) << to_string(ta.timer);

    ta.update(1/60.0);
    /* Frame should have advanced */
    ASSERT_EQ(2, ta.current_tile_id()) << "Timer = " << to_string(ta.timer);

    ta.update(1/60.0);
    ta.update(1/60.0);
    ASSERT_EQ(3, ta.current_tile_id());

    /* Should reset to 0 at end of animation */
    ta.update(1/60.0);
    ta.update(1/60.0);
    ASSERT_EQ(1, ta.current_tile_id());


};
}
