

#include "collision.h"
#include <pixel/error.h>

namespace pixel::collision
{

uint8_t byte_with_ones(uint num_ones)
{
    return (1 << num_ones) - 1;
}

bool CollisionMap::collide_row(uint row, uint start, uint stop)
{
    argument_error_if(row >= map_height_, "row out of range");
    argument_error_if(start >= map_width_, "start_col out of range");
    argument_error_if(stop >= map_width_, "end_col out of range");

    argument_error_if(start > stop, "argument start must be less than or equal argument stop");
    argument_error_if(stop - start > 8, "checking spans greater than 8 tiles is not supported");

    uint8_t collide_mask = byte_with_ones(stop - start + 1);

    auto bit_offset = start & 0b111;
    auto bitmap_col = start / 8;

    uint8_t map_mask = bitmap_rows_[bitmap_col + row * bitmap_width_] >> bit_offset;

    // fill top bits of map_mask with tile values from next adjacent byte in bitmap
    if (bit_offset > 0 && (bitmap_col + 1) < bitmap_width_) {
        map_mask |= (bitmap_rows_[(bitmap_col + 1) + row * bitmap_width_]) << (8 - bit_offset);
    }

    return (map_mask & collide_mask) > 0;
}

bool CollisionMap::collide_column(uint col, uint start, uint stop)
{
    argument_error_if(col >= map_width_, "row out of range");
    argument_error_if(start >= map_height_, "start_col out of range");
    argument_error_if(stop >= map_height_, "end_col out of range");

    argument_error_if(start > stop, "argument start must be less than or equal argument stop");
    argument_error_if(stop - start > 8, "checking spans greater than 8 tiles is not supported");

    uint8_t collide_mask = byte_with_ones(stop - start + 1);

    auto bit_offset = start & 0b111;
    auto bitmap_row = start / 8;

    uint8_t map_mask = bitmap_columns_[bitmap_row + col * bitmap_height_] >> bit_offset;

    // fill top bits of map_mask with tile values from next adjacent byte in bitmap
    if (bit_offset > 0 && (bitmap_row + 1) < bitmap_height_) {
        map_mask |= (bitmap_columns_[(bitmap_row + 1) + col * bitmap_height_]) << (8 - bit_offset);
    }

    return (map_mask & collide_mask) > 0;
}

CollisionMap::CollisionMap(uint width, uint height)
{
    bitmap_width_ = width / 8 + ((width & 0b111) > 0 ? 1 : 0);
    bitmap_height_ = height / 8 + ((height & 0b111) > 0 ? 1 : 0);

    map_width_ = width;
    map_height_ = height;

    bitmap_rows_.resize(bitmap_width_ * map_height_);
    bitmap_columns_.resize(bitmap_height_ * map_width_);
}

void CollisionMap::set(int x, int y, bool solid)
{
    argument_error_if(x < 0 || x >= map_width_, "argument x out of range");
    argument_error_if(y < 0 || y >= map_height_, "argument y out of range");

    if (solid) {
        bitmap_rows_[x / 8 + y * bitmap_width_] |= 1 << (x % 8);
        bitmap_columns_[y / 8 + x * bitmap_height_] |= 1 << (y % 8);
    } else {
        bitmap_rows_[x / 8 + y * bitmap_width_] &= ~(1 << (x % 8));
        bitmap_columns_[y / 8 + x * bitmap_height_] &= ~(1 << (y % 8));
    }
}

}

