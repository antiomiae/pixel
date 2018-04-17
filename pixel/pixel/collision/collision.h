#ifndef PIXEL_COLLISION_H
#define PIXEL_COLLISION_H

#include <vector>
#include <pixel/types.h>

namespace pixel::collision
{

using namespace std;

class CollisionMap
{
public:
    CollisionMap() = default;
    CollisionMap(uint width, uint height);

    bool collide_row(uint row, uint start_col, uint end_col);
    bool collide_column(uint col, uint a, uint b);

private:
    vector<uint8_t> row_major_bitmap_;
    vector<uint8_t> column_major_bitmap_;
    uint bitmap_width_;
    uint bitmap_height_;
    uint map_width_;
    uint map_height_;
};

};

#endif //PIXEL_COLLISION_H
