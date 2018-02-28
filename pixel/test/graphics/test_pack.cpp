#include "test/setup.h"
#include <pixel/graphics/pack.h>

namespace
{


}

TEST(Pack, pack_rects_array)
{
    struct Rect
    {
        unsigned w, h, id;
    };

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

    for (auto row : packing) {
        auto rect = get<0>(row);
        auto coord = get<1>(row);
        cout << "{w = " << rect.w << ", h = " << rect.h << ", id = " << rect.id << "} => ";
        cout << "{ x = " << coord.x << ", y = " << coord.y << ", z = " << coord.z << "}" << endl;
    }
}

};

