#include <pixel/pixel.h>
#include "test/setup.h"
namespace
{
TEST(TileAnimation, Animation)
{
    pixel::TileLayer::TileAnimation ta;

    ta.animation_definition.frames = {
        {1, 32},
        {2, 32},
        {3, 32}
    };

    /* Should start at beginning of sequence */
    ASSERT_EQ(0, ta.frame);
    ASSERT_EQ(0, ta.timer);

    ta.update(16);
    /* Should still be on first frame */
    ASSERT_EQ(0, ta.frame);

    ta.update(16);
    /* Frame should have advanced */
    ASSERT_EQ(1, ta.frame);

    ta.update(16);
    ta.update(16);
    ASSERT_EQ(2, ta.frame);

    /* Should reset to 0 at end of animation */
    ta.update(16);
    ta.update(16);
    ASSERT_EQ(0, ta.frame);


};
}
