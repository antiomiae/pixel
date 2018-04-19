#include <iostream>
#include "test/setup.h"
#include <pixel/graphics/pack.h>


namespace
{

using namespace pixel;
using namespace pixel::pack;
using namespace std;

struct Rect
{
    unsigned w, h, id;
};

TEST(pack_rects_array, pack_rects_array)
{


    vector<Rect> test_data = {
        {100, 100, 1},
        {90,  100, 2},
        {80,  80,  3},
        {80,  70,  4},
        {90,  100, 8},
        {80,  80,  3},
        {80,  70,  4},
        {40,  60,  5},
        {30,  50,  6},
        {20,  4,   7},
    };

    {
        int id = 0;
        std::for_each(begin(test_data), end(test_data), [&](auto& r) { r.id = id++; });
    }

    auto [packing, leftover] = pixel::pack::pack_rects_array(test_data, 150, 150, 5);
    EXPECT_EQ(test_data.size(), packing.size());
    EXPECT_EQ(0, leftover.size());
}

TEST(pack_rects_array, throws_with_bad_arguments)
{
    auto test_data = vector<Rect>{};

    EXPECT_ANY_THROW(
        pixel::pack::pack_rects_array(test_data, 0, 1, 1)
    );

    EXPECT_ANY_THROW(
        pixel::pack::pack_rects_array(test_data, 1, 0, 1)
    );

    EXPECT_ANY_THROW(
        pixel::pack::pack_rects_array(test_data, 1, 1, 0)
    );

    EXPECT_ANY_THROW(
        pixel::pack::pack_rects_array(test_data, 0, 0, 0)
    );
}

};

