#ifndef PIXEL_MAIN_TILE_MAP_COLLIDER_H
#define PIXEL_MAIN_TILE_MAP_COLLIDER_H

#include <limits>
#include <algorithm>
#include <array>
#include <pixel/math.h>
#include <pixel/tilemap/tile_layer.h>

namespace pixel
{

struct BoundingBox
{
    glm::vec2 start_position{};
    glm::vec2 end_position{};
    glm::vec2 size{};
};

template<class T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

struct CollisionRect
{
    glm::vec2 center;
    glm::vec2 half_size;
    glm::vec2 delta;
    glm::ivec2 tile_size;

    CollisionRect() = default;

    CollisionRect(glm::vec2 center, glm::vec2 half_size, glm::vec2 delta, glm::ivec2 tile_size)
        : center(center),
          half_size(half_size),
          delta(delta),
          tile_size(tile_size)
    {

    }

    int nearest_tile_col()
    {
        auto sign = sgn(delta.x);
        float x = center.x + sign * (half_size.x + tile_size.x - 1);
        return (int) (delta.x > 0 ? ceil(x) : x) / tile_size.x;
    }

    int nearest_tile_row()
    {
        auto sign = sgn(delta.y);
        float y = center.y + sign * (half_size.y + tile_size.y - 1);
        return (int) (delta.y > 0 ? ceil(y) : y) / tile_size.y;
    }

    float edge_x()
    {
        auto sign = sgn(delta.x);
        return center.x + sign * half_size.x;
    }

    float edge_y()
    {
        auto sign = sgn(delta.y);
        return center.y + sign * half_size.y;
    }

    float distance_to_tile_x(int tile_col)
    {

        auto half_tile = tile_size.x / 2.0f;
        auto tile_center = tile_col * tile_size.x + half_tile;
        auto r = half_size.x + tile_size.x / 2.0f;
        auto d = tile_center - center.x + (tile_center >= center.x ? -1 : 1) * r;

        return d;
    }

    float distance_to_tile_y(int tile_row)
    {

        auto half_tile = tile_size.y / 2.0f;
        auto tile_center = tile_row * tile_size.y + half_tile;
        auto r = half_size.y + tile_size.y / 2.0f;
        auto d = tile_center - center.y + (tile_center >= center.y ? -1 : 1) * r;

        return d;
    }

    float intersection_time_x(int col)
    {
        auto d = distance_to_tile_x(col);
        auto t = d / delta.x;

        return t;
    }

    float intersection_time_y(int row)
    {
        auto d = distance_to_tile_y(row);
        auto t = d / delta.y;

        return t;
    }
};

enum class CheckCollisionAxis
{
    None,
    Vertical,
    Horizontal
};


struct TileMapCollider
{
    static glm::ivec2 collide(
        BoundingBox& object,
        TileLayer& tile_layer,
        const function<bool(TileLayer::Tile & , Tileset::Tile & )>& tile_callback,
        bool slide = false
    )
    {
        auto delta = object.end_position - object.start_position;

        /* return early if object isn't moving */
        if (delta == glm::vec2(0, 0)) {
            return {0, 0};
        }

        auto& parent = tile_layer.parent();
        auto test_rect = CollisionRect(
            object.start_position + object.size / 2.0f, object.size / 2.0f,
            delta, parent.tile_size());

        glm::ivec2 tile_count = parent.tile_count();

        /*
         * Sweep rectangle through the map:
         *  1. solve for collision time between an axis of the object and the tile grid
         *  2. advance object along path for shortest time
         *  3. check tiles at grid line for solidity
         *  4.
         */

        float acc_t = 0;

        glm::ivec2 collision_index = {-1, -1};
        glm::ivec2 collision_axes = {0, 0};

        array<CheckCollisionAxis, 2> axes_to_check;

        glm::ivec2 delta_sign = { sgn(delta.x), sgn(delta.y) };

        while (delta != glm::vec2(0.0f, 0.0f)) {
            float xt = numeric_limits<float>::infinity();
            float yt = numeric_limits<float>::infinity();

            auto original_center = test_rect.center;
            auto original_delta = test_rect.delta;

            // Ensure delta hasn't flipped around
            if (delta.x != 0) {
                assert(sgn(delta.x) == delta_sign.x);
            }

            if (delta.y != 0) {
                assert(sgn(delta.y) == delta_sign.y);
            }

            if (test_rect.delta.x != 0) {
                int col = test_rect.nearest_tile_col();
                assert(col >= 0);

                // if we checked this column last frame, check the next column instead
                if (collision_index.x == col) {
                    collision_index.x = col + sgn(test_rect.delta.x);
                    assert(col >= 0);
                } else {
                    collision_index.x = col;
                }

                xt = test_rect.intersection_time_x(collision_index.x);

                assert(xt >= 0);
            }

            if (test_rect.delta.y != 0) {
                int row = test_rect.nearest_tile_col();
                assert(row >= 0);

                // if we checked this column last frame, check the next column instead
                if (collision_index.y == row) {
                    collision_index.y = row + sgn(test_rect.delta.y);
                    assert(row >= 0);
                } else {
                    collision_index.y = row;
                }

                yt = test_rect.intersection_time_y(collision_index.y);

                assert(yt >= 0);
            }

            if (xt >= 1 && yt >= 1) {
                // advance test_rect
                test_rect.center += test_rect.delta;
                break;
            }

            if (xt < 1) {
                if (yt <= xt) {
                    axes_to_check = {CheckCollisionAxis::Vertical, CheckCollisionAxis::Horizontal};
                } else {
                    axes_to_check[0] = CheckCollisionAxis::Horizontal;

                    if (yt < 1) {
                        axes_to_check[1] = CheckCollisionAxis::Vertical;
                    }
                }
            } else if (yt < 1) {
                axes_to_check[0] = CheckCollisionAxis::Vertical;
            }

            switch (axes_to_check[0]) {
                case CheckCollisionAxis::Horizontal: {

                    auto d = xt * test_rect.delta;
                    delta = original_delta - d;
                    test_rect.center = original_center + d;

                    auto column_to_check = collision_index.x;
                    auto row_span = glm::ivec2(
                        test_rect.center.y - test_rect.half_size.y, test_rect.center.y + test_rect.half_size.y
                    ) / test_rect.tile_size.y;

                    if (check_solid_column(tile_layer, column_to_check, row_span, tile_callback)) {
                        delta.x = 0;
                        collision_axes.x = sgn(test_rect.delta.x);

                        if (!slide) {
                            axes_to_check[1] = CheckCollisionAxis::None;
                            delta.y = 0;
                        }
                    }
                }

                    cout << "Axis Check 0: Horizontal" << endl;
                    break;
                case CheckCollisionAxis::Vertical: {
                    auto d = yt * test_rect.delta;
                    delta = original_delta - d;
                    test_rect.center = original_center + d;

                    auto row_to_check = collision_index.y;
                    auto column_span = glm::ivec2(
                        test_rect.center.x - test_rect.half_size.x, test_rect.center.x + test_rect.half_size.x
                    ) / test_rect.tile_size.x;

                    if (check_solid_row(tile_layer, row_to_check, column_span, tile_callback)) {
                        delta.y = 0;
                        collision_axes.y = sgn(test_rect.delta.y);

                        if (!slide) {
                            axes_to_check[1] = CheckCollisionAxis::None;
                            delta.x = 0;
                        }
                    }
                }

                    cout << "Axis Check 0: Vertical" << endl;
                    break;

                default:
                    cout << "Axis Check 0: None" << endl;
                    break;
            }

            switch (axes_to_check[1]) {
                case CheckCollisionAxis::Horizontal: {
                    auto d = xt * test_rect.delta;
                    delta = original_delta - d;
                    test_rect.center = original_center + d;

                    auto column_to_check = collision_index.x;
                    auto row_span = glm::ivec2(
                        test_rect.center.y - test_rect.half_size.y, test_rect.center.y + test_rect.half_size.y
                    ) / test_rect.tile_size.y;

                    if (check_solid_column(tile_layer, column_to_check, row_span, tile_callback)) {
                        delta.x = 0;
                        collision_axes.x = sgn(test_rect.delta.x);

                        if (!slide) {
                            delta.y = 0;
                        }
                    }
                }
                    cout << "Axis Check 1: Horizontal" << endl;

                    break;
                case CheckCollisionAxis::Vertical: {
                    auto d = yt * test_rect.delta;
                    delta = original_delta - d;
                    test_rect.center = original_center + d;

                    auto row_to_check = collision_index.y;
                    auto column_span = glm::ivec2(
                        test_rect.center.x - test_rect.half_size.x, test_rect.center.x + test_rect.half_size.x
                    ) / test_rect.tile_size.x;

                    if (check_solid_row(tile_layer, row_to_check, column_span, tile_callback)) {
                        delta.y = 0;
                        collision_axes.y = sgn(test_rect.delta.y);

                        if (!slide) {
                            delta.x = 0;
                        }
                    }
                }

                    cout << "Axis Check 1: Vertical" << endl;
                    break;
                default:

                    cout << "Axis Check 1: None" << endl;
                    break;
            }

            // reset for next round
            test_rect.delta = delta;
            axes_to_check = {CheckCollisionAxis::None, CheckCollisionAxis::None};
        }

        object.end_position = test_rect.center - test_rect.half_size;

        return collision_axes;
    }

    bool static check_solid_column(
        TileLayer& tile_layer,
        int column,
        glm::ivec2 row_span,
        const function<bool(TileLayer::Tile & , Tileset::Tile & )>& tile_callback
    )
    {
        auto collided = false;

        tile_layer.visit_tiles(
            TileCoordinate(column, row_span.s),
            TileCoordinate(column, row_span.t),

            [&](auto tile_coord, auto& tile) -> auto {
                if (tile.tile_id != 0) {
                    auto& tile_desc = tile_layer.parent().tileset().tile(tile.tile_id);

                    if (tile_callback(tile, tile_desc)) {

                        collided = true;

                        return false;
                    }
                }

                return true;
            }
        );

        return collided;
    }


    bool static check_solid_row(
        TileLayer& tile_layer,
        int row,
        glm::ivec2 col_span,
        const function<bool(TileLayer::Tile & , Tileset::Tile & )>& tile_callback
    )
    {
        auto collided = false;

        tile_layer.visit_tiles(
            TileCoordinate(col_span.s, row),
            TileCoordinate(col_span.t, row),

            [&](auto tile_coord, auto& tile) -> auto {
                if (tile.tile_id != 0) {
                    auto& tile_desc = tile_layer.parent().tileset().tile(tile.tile_id);

                    if (tile_callback(tile, tile_desc)) {

                        collided = true;

                        return false;
                    }
                }

                return true;
            }
        );

        return collided;
    }

};

};

#endif //PIXEL_MAIN_TILE_MAP_COLLIDER_H
