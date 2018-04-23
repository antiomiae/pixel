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
    CollisionMap(uint width, uint height);

    void set(int x, int y, bool solid);

    bool collide_row(uint row, uint start, uint stop);
    bool collide_column(uint col, uint start, uint stop);

private:
    vector<uint8_t> bitmap_rows_;
    vector<uint8_t> bitmap_columns_;
    uint bitmap_width_;
    uint bitmap_height_;
    uint map_width_;
    uint map_height_;
};

};

#endif //PIXEL_COLLISION_H
