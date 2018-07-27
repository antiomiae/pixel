#ifndef PIXEL_MAIN_TILE_MAP_COLLIDER_H
#define PIXEL_MAIN_TILE_MAP_COLLIDER_H

#include <limits>
#include <algorithm>
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

template <class T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

struct CollisionRect2
{
    glm::vec2 center;
    glm::vec2 half_size;
    glm::vec2 delta;
    glm::ivec2 tile_size;

    int nearest_tile_col()
    {
        auto sign = sgn(delta.x);
        float x = center.x + sign*(half_size.x + tile_size.x - 1);
        return (int) (delta.x > 0 ? x : floor(x)) / tile_size.x;
    }

    int nearest_tile_row()
    {
        auto sign = sgn(delta.y);
        float y = center.y + sign*(half_size.y + tile_size.y - 1);
        return (int)(delta.y > 0 ? y : floor(y)) / tile_size.y;
    }

    float edge_x()
    {
        auto sign = sgn(delta.x);
        return center.x + sign*half_size.x;
    }

    float edge_y()
    {
        auto sign = sgn(delta.y);
        return center.y + sign*half_size.y;
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

        auto half_tile = tile_size.x / 2.0f;
        auto tile_center = tile_row * tile_size.y + half_tile;
        auto r = half_size.y + tile_size.y / 2.0f;
        auto d = tile_center - center.y + (delta.y > 0 ? -1 : 1) * r;

        return d;
    }

    float intersection_time_x()
    {
        auto col = nearest_tile_col();
        auto d = distance_to_tile_x(col);
        auto t = d / delta.x;

        return t;
    }

    float intersection_time_y()
    {
        auto row = nearest_tile_row();
        auto d = distance_to_tile_y(row);
        auto t = d / delta.y;

        return t;
    }
};

struct TileMapCollider
{
    struct CollisionRect
    {
        glm::vec2 position;
        glm::vec2 size;

        CollisionRect() = default;

        CollisionRect(glm::vec2 p, glm::vec2 s)
            : position(p),
              size(s)
        {
        }
    };

    static void visit_tiles(
        TileLayer& tile_layer,
        TileCoordinate p0,
        TileCoordinate p1,
        const function<bool(TileCoordinate, TileLayer::Tile&)>& cb
    )
    {
        for (auto y = min(p0.y, p1.y); y <= max(p0.y, p1.y); ++y) {
            for (auto x = min(p0.x, p1.x); x <= max(p0.x, p1.x); ++x) {
                auto should_continue = cb(TileCoordinate(x, y), tile_layer.at(x, y));

                if (!should_continue) {
                    return;
                }
            }
        }
    }

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

        auto delta_inv = 1.0f / delta;

        auto& parent = tile_layer.parent();

        glm::ivec2 tile_size = parent.tile_size();
        glm::ivec2 tile_count = parent.tile_count();

        auto test_rect = CollisionRect(object.start_position, object.size);

        glm::ivec2 dir = {
            delta.x > 0 ? 1 : (delta.x < 0 ? -1 : 0),
            delta.y > 0 ? 1 : (delta.y < 0 ? -1 : 0)
        };

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

        while (delta != glm::vec2{0, 0}) {
            float ct = std::numeric_limits<float>::infinity();
            bool check_column = false;
            bool check_row = false;

            float edge_x = -1;
            float edge_y = -1;
            float grid_x = -1;
            float grid_y = -1;

            if (delta.x != 0) {
                edge_x = test_rect.position.x + (dir.x == 1 ? test_rect.size.x : 0);

                int idx;
                if (edge_x <= 0 && dir.x > 0) {
                    idx = 0;
                } else {
                    auto pre_index = (int) ((dir.x > 0 ? floor(edge_x - 1) : floor(edge_x)) / (int) tile_size.x);
                    idx = pre_index + (dir.x == 1 ? 1 : 0);
                }

                // we've already checked this gridline and decided to go through it
                if (collision_index.x == idx) {
                    collision_index.x += dir.x;
                } else {
                    collision_index.x = idx;
                }

                // ensure we're going to collide with the map
                if (collision_index.x >= (dir.x > 0 ? 0 : 1) &&
                    collision_index.x <= (dir.x == 1 ? tile_count.x : tile_count.x)) {
                    grid_x = collision_index.x * tile_size.x;
                    float d = grid_x - edge_x;

                    ct = d * delta_inv.x;
                    check_column = true;
                }
            }

            if (delta.y != 0) {
                edge_y = test_rect.position.y + (dir.y == 1 ? test_rect.size.y : 0);

                int idx;
                if (edge_y <= 0 && dir.y > 0) {
                    idx = 0;
                } else {
                    auto pre_index = (int) ((dir.y > 0 ? floor(edge_y - 1) : floor(edge_y)) / (int) tile_size.y);
                    idx = pre_index + (dir.y == 1 ? 1 : 0);
                }

                if (collision_index.y == idx) {
                    collision_index.y += dir.y;
                } else {
                    collision_index.y = idx;
                }

                // ensure we're going to collide with the map
                if (collision_index.y >= (dir.y == 1 ? 0 : 1) &&
                    collision_index.y <= (dir.y > 0 ? tile_count.y - 1 : tile_count.y)) {
                    grid_y = collision_index.y * tile_size.y;
                    float yt = (grid_y - edge_y) * delta_inv.y;

                    if (yt <= ct) {
                        if (yt < ct) {
                            check_column = false;
                        }

                        ct = yt;
                        check_row = true;
                    }
                }
            }

            // if ct >= 1, consume remaining delta and return
            if (ct > 1) {
                test_rect.position += delta;
                delta = {0, 0};
                break;
            }

            auto d = ct * delta;

            if (check_column) {
                if (dir.x > 0) {
                    test_rect.position.x = collision_index.x * tile_size.x - test_rect.size.x;
                } else {
                    test_rect.position.x = collision_index.x * tile_size.x;
                }
            } else {
                collision_index.x = -1;
                test_rect.position.x += d.x;
            }

            if (check_row) {
                if (dir.y > 0) {
                    test_rect.position.y = collision_index.y * tile_size.y - test_rect.size.y;
                } else {
                    test_rect.position.y = collision_index.y * tile_size.y;
                }
            } else {
                collision_index.y = -1;
                test_rect.position.y += d.y;
            }

            delta -= d;
            delta_inv = 1.0f / delta;

            if (check_column) {
                auto column = collision_index.x - (dir.x < 0 ? 1 : 0);
                error_if(column < 0, "column index less than 0");

                int ymin = glm::clamp((int) floor(test_rect.position.y), 0, tile_count.y * tile_size.y - 1);

                int ymax = glm::clamp((int) ceil(test_rect.position.y + test_rect.size.y - 1), 0,
                                      tile_count.y * tile_size.y - 1
                );


                if (ymin < ymax) {

                    glm::ivec2 row_span = {
                        ymin / tile_size.y,
                        ymax / tile_size.y
                    };

                    if (dir.y != 0 && check_row) {
                        if (dir.y > 0) {
                            // also check the tile above the range calculated if moving up
                            row_span.t += 1;
                        } else {
                            // or the tile below the range if moving down
                            row_span.s -= 1;
                        }
                    }

                    row_span = glm::clamp(row_span, 0, tile_count.y - 1);

                    visit_tiles(
                        tile_layer,
                        TileCoordinate(column, row_span.s),
                        TileCoordinate(column, row_span.t),

                        [&](auto tile_coord, auto& tile) -> auto {
                            if (tile.tile_id != 0) {
                                auto& tile_desc = parent.tileset().tile(tile.tile_id);

                                if (tile_callback(tile, tile_desc)) {

                                    collision_axes.x = dir.x;
                                    //dir.x = 0;

                                    delta.x = 0;

                                    if (!slide) {
                                        delta.y = 0;
                                    }

                                    return false;
                                }
                            }

                            return true;
                        }
                    );

                }
            }

            if (check_row) {
                auto row = collision_index.y - (dir.y < 0 ? 1 : 0);
                error_if(row < 0, "row index less than 0");

                int xmin = glm::clamp((int) floor(test_rect.position.x), 0, tile_count.x * tile_size.x - 1);
                int xmax = glm::clamp((int) ceil(test_rect.position.x + test_rect.size.x - 1), 0,
                                      tile_count.x * tile_size.x - 1
                );

                if (xmin < xmax) {

                    auto col_span = glm::ivec2(xmin / tile_size.x, xmax / tile_size.x);

                    if (dir.x != 0 && check_column) {
                        if (dir.x > 0) {
                            // also check the tile right of the range calculated if moving right
                            col_span.t += 1;
                        } else {
                            // or the left tile if moving left
                            col_span.s -= 1;
                        }
                    }

                    col_span = glm::clamp(col_span, 0, tile_count.x - 1);

                    visit_tiles(
                        tile_layer,
                        TileCoordinate(col_span.s, row),
                        TileCoordinate(col_span.t, row),

                        [&](auto tile_coord, auto& tile) -> auto {
                            if (tile.tile_id != 0) {
                                auto& tile_desc = parent.tileset().tile(tile.tile_id);

                                /* */
                                if (tile_callback(tile, tile_desc)) {
                                    collision_axes.y = dir.y;
                                    //dir.y = 0;
                                    delta.y = 0;

                                    if (!slide) {
                                        delta.x = 0;
                                    }

                                    return false;
                                }
                            }

                            return true;
                        }
                    );

                }
            }

            if (abs(delta.x) <= 0.0001f) {
                delta.x = 0;
                dir.x = 0;
            }

            if (abs(delta.y) <= 0.0001f) {
                delta.y = 0;
                dir.y = 0;
            }
        }

        object.end_position = test_rect.position;

        return collision_axes;
    }
};

};

#endif //PIXEL_MAIN_TILE_MAP_COLLIDER_H
