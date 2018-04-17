

#include "collision.h"
#include <pixel/error.h>

namespace pixel::collision
{

uint8_t byte_with_ones(uint num_ones)
{
    return (1 << num_ones) - 1;
}

bool CollisionMap::collide_row(uint row, uint start_col, uint end_col)
{
    argument_error_if(row < 0 || row >= map_height_, "row out of range");
    argument_error_if(start_col < 0 || start_col >= map_width_, "start_col out of range");
    argument_error_if(end_col < 0 || end_col >= map_width_, "end_col out of range");

    argument_error_if(start_col >= end_col, "argument a must be less than argument b");
    argument_error_if(end_col - start_col > 8, "checking spans greater than 8 tiles is not supported");

    uint8_t collide_mask = byte_with_ones(end_col - start_col);

    auto bit_offset = start_col & 111b;
    auto bitmap_col = start_col / 8;

    uint8_t map_mask = row_major_bitmap_[bitmap_col + row * bitmap_width_] >> bit_offset;

    // fill top bits of map_mask with tile values from next adjacent byte in bitmap
    if (bit_offset > 0 && (bitmap_col + 1) < bitmap_width_) {
        map_mask |= (row_major_bitmap_[(bitmap_col + 1) + row * bitmap_width_]) << (8 - bit_offset);
    }

    return (map_mask & collide_mask) > 0;
}

bool CollisionMap::collide_column(uint col, uint a, uint b)
{
    return false;
}

CollisionMap::CollisionMap(uint width, uint height)
{
    bitmap_width_ = width / 8 + (width & 111b > 0 ? 1 : 0);
    bitmap_height_ = height / 8 + (height & 111b > 0 ? 1 : 0);

    map_width_ = width;
    map_height_ = height;

    row_major_bitmap_.resize(bitmap_width_ * map_height_);
    column_major_bitmap_.resize(bitmap_height_ * map_width_);
}

void CollisionMap::set(int x, int y, bool solid)
{
    argument_error_if(x < 0 || x >= map_width_, "argument x out of range");
    argument_error_if(y < 0 || y >= map_height_, "argument y out of range");

    row_major_bitmap_[x / 8 + y * bitmap_width_] &= 1 << (x % 8);
    column_major_bitmap_[y / 8 + x * bitmap_height_] &= 1 << (y % 8);
}

}

