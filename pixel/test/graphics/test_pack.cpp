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

TEST_CASE("pack_rects_array")
{
    SECTION("it packs the rects") {
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

        auto[packing, leftover] = pixel::pack::pack_rects_array(test_data, 150, 150, 5);

        REQUIRE(test_data.size() == packing.size());
        REQUIRE(0 == leftover.size());
    }

    SECTION("throws_with_bad_arguments") {
        auto test_data = vector<Rect>{};

        REQUIRE_THROWS(
            pixel::pack::pack_rects_array(test_data, 0, 1, 1)
        );

        REQUIRE_THROWS(
            pixel::pack::pack_rects_array(test_data, 1, 0, 1)
        );

        REQUIRE_THROWS(
            pixel::pack::pack_rects_array(test_data, 1, 1, 0)
        );

        REQUIRE_THROWS(
            pixel::pack::pack_rects_array(test_data, 0, 0, 0)
        );
    }
}

};
