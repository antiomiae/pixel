#include <experimental/optional>
#include <limits>
#include <algorithm>

#include <pixel/pixel.h>
#include <pixel/level.h>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/fast_square_root.hpp>
#include <glm/gtx/string_cast.hpp>


using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;


struct BoundingBox
{
    glm::vec2 start_position{};
    glm::vec2 end_position{};
    glm::vec2 size{};
};

struct TileMapCollider
{
    struct Overlap
    {

    };

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

    static glm::ivec2
    collide(BoundingBox& object, TileLayer& tile_layer, const function<bool(TileLayer::Tile&, Tileset::Tile&)>& tile_callback)
    {
        auto& parent = tile_layer.parent();
        glm::ivec2 tile_size = parent.tile_size();
        glm::ivec2 tile_count = parent.tile_count();

        auto delta = object.end_position - object.start_position;

        /* return early if object isn't moving */
        if (delta == glm::vec2(0, 0)) {
            return {0, 0};
        }

        auto delta_inv = 1.0f / delta;

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

            if (delta.x != 0) {
                float edge = test_rect.position.x + (dir.x == 1 ? test_rect.size.x - 1: 0);

                int idx;
                if (edge <= 0 && dir.x > 0) {
                    idx = 0;
                } else {
                    idx = (int) (dir.x > 0 ? ceil(edge) : floor(edge)) / (int) tile_size.x + (dir.x == 1 ? 1 : 0);
                }

                // we've already checked this gridline and decided to go through it
                if (collision_index.x == idx) {
                    collision_index.x += dir.x;
                } else {
                    collision_index.x = idx;
                }

                // ensure we're going to collide with the map
                if (collision_index.x >= (dir.x > 0 ? 0 : 1) && collision_index.x <= (dir.x == 1 ? tile_count.x - 1 : tile_count.x)) {
                    float grid_x = collision_index.x * tile_size.x - (dir.x == 1 ? 1 : 0);
                    float d = grid_x - edge;

                    ct = d * delta_inv.x;
                    check_column = true;
                }
            }

            if (delta.y != 0) {
                float edge = test_rect.position.y + (dir.y == 1 ? test_rect.size.y - 1 : 0);

                int idx;
                if (edge <= 0 && dir.y > 0) {
                    idx = 0;
                } else {
                    idx = (int) (dir.y > 0 ? ceil(edge) : floor(edge)) / (int) tile_size.y + (dir.y == 1 ? 1 : 0);
                }

                if (collision_index.y == idx) {
                    collision_index.y += dir.y;
                } else {
                    collision_index.y = idx;
                }

                // ensure we're going to collide with the map
                if (collision_index.y >= (dir.y == 1 ? 0 : 1) && collision_index.y <= (dir.y > 0 ? tile_count.y - 1 : tile_count.y)) {
                    float grid_y = collision_index.y * tile_size.y - (dir.y == 1 ? 1 : 0);
                    float yt = (grid_y - edge) * delta_inv.y;

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
                test_rect.position.x += d.x;
            }

            if (check_row) {
                if (dir.y > 0) {
                    test_rect.position.y = collision_index.y * tile_size.y - test_rect.size.y;
                } else {
                    test_rect.position.y = collision_index.y * tile_size.y;
                }
            } else {
                test_rect.position.y += d.y;
            }

            delta -= d;
            delta_inv = 1.0f / delta;

            if (check_column) {
                auto column = collision_index.x - (dir.x < 0 ? 1 : 0);

                glm::ivec2 y_span = {
                    floor(test_rect.position.y),
                    ceil(test_rect.position.y + test_rect.size.y),
                };

                y_span = glm::clamp(y_span, 0, tile_count.y * tile_size.y - 1);

                error_if(column < 0, "column index less than 0");

                if (y_span.s < y_span.t) {

                    glm::uvec2 row_span = {
                        y_span.s / tile_size.y,
                        y_span.t / tile_size.y
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

                    row_span = glm::clamp(row_span, 0u, tile_count.y - 1u);

                    for (auto y = row_span.s; y <= row_span.t; ++y) {
                        auto& tile = tile_layer.at(column, y);

                        if (tile.tile_id != 0) {
                            auto& tile_desc = parent.tileset().tile(tile.tile_id);

                            if (tile_callback(tile, tile_desc)) {

                                collision_axes.x = dir.x;
                                dir.x = 0;

                                // should take which axes to stop as parameter, or by callback return value
                                delta = {0, 0};

                                break;
                            }
                        }
                    }
                }
            }

            if (check_row) {
                auto row = collision_index.y - (dir.y < 0 ? 1 : 0);

                glm::ivec2 x_span = {
                    floor(test_rect.position.x),
                    ceil(test_rect.position.x + test_rect.size.x),
                };

                x_span = glm::clamp(x_span, 0, tile_count.x * tile_size.x - 1);

                error_if(row < 0, "row index less than 0");

                if (x_span.s < x_span.t) {

                    glm::uvec2 col_span = {
                        x_span.s / tile_size.x,
                        x_span.t / tile_size.x
                    };

                    col_span = glm::clamp(col_span, 0u, tile_count.x - 1u);

                    for (auto x = col_span.s; x <= col_span.t; ++x) {
                        auto& tile = tile_layer.at(x, row);

                        if (tile.tile_id != 0) {
                            auto& tile_desc = parent.tileset().tile(tile.tile_id);

                            if (tile_callback(tile, tile_desc)) {
                                collision_axes.y = dir.y;
                                dir.y = 0;
                                delta = {0, 0};

                                break;
                            }
                        }
                    }
                }
            }

            if (abs(delta.x) <= 0.0001f) {
                delta.x = 0;
            }

            if (abs(delta.y) <= 0.0001f) {
                delta.y = 0;
            }
        }

        object.end_position = test_rect.position;

        return collision_axes;
    }
};

float clamp(float x, float min_, float max_)
{
    return max(min(x, max_), min_);
}

struct Guy
{

    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acc{1.f, 1.f};

    bool on_ground;
    int layer{0};
    Level* level;

    Sprite sprite;

    Guy(Level* level, const string& sprite_name)
        : level{level}
    {
        sprite = level->get_sprite(sprite_name);
    }

    void update(float dt)
    {
        if (input::Keyboard::keymap[GLFW_KEY_RIGHT]) {
            velocity.x += acc.x;
        } else if (input::Keyboard::keymap[GLFW_KEY_LEFT]) {
            velocity.x -= acc.x;
        } else {
            if (velocity.x != 0) {
                velocity.x += acc.x * (velocity.x > 0 ? -1 : 1);
            }

            if (abs(velocity.x) < acc.s) {
                velocity.x = 0;
            }
        }

        if (input::Keyboard::keymap[GLFW_KEY_UP]) {
            velocity.y += acc.s;
        } else if (input::Keyboard::keymap[GLFW_KEY_DOWN]) {
            velocity.y -= acc.s;
        } else {
            if (velocity.y > 0 || velocity.y < 0) {
                velocity.y += acc.s * (velocity.y > 0 ? -1 : 1);
            }

            if (abs(velocity.y) < acc.s) {
                velocity.y = 0;
            }
        }

        velocity = glm::clamp(velocity, -10.0f, 10.0f);

        BoundingBox b;
        b.start_position = position;
        b.end_position = position + velocity;
        b.size = {sprite.texture_region.w, sprite.texture_region.h};

        auto collision_axes = TileMapCollider::collide(
            b,
            level->tile_map().layers()[layer],
            [&](auto& tile, auto& tile_desc) {
                auto type = tile_desc.type;
                return type == "brick";
            }
        );

        position = b.end_position;

        if (collision_axes != glm::ivec2(0, 0)) {
            velocity = {0, 0};
        }
    }

    void render(SpriteBatch& batch)
    {
        sprite.position = position;
        batch.add(sprite);
    }
};


void run(Level& level)
{
    level.load_sprites({"assets/guy.png"});
    level.load_map("assets/map.tmx");

    Guy guy{&level, "assets/guy.png"};
    guy.layer = 2;
    guy.acc = {0.25f, 0.f};

    SpriteBatch batch;

    pixel::app().set_tick_callback(
        [&] {
            static bool flip = false;

            //if (input::Keyboard::keymap[GLFW_KEY_SPACE] && !flip) {
                guy.update(1 / 60.0f);
//                flip = true;
//            } else if (!input::Keyboard::keymap[GLFW_KEY_SPACE] && flip) {
//                flip = false;
//            }

            level.begin_render();

            batch.restart();

            level.renderer_group().get<renderers::TileMapRenderer>().render(level.tile_map(), level.camera());

            guy.render(batch);

            level.renderer_group()
                .get<renderers::SpriteRenderer>()
                .render(
                    batch.sprites(), level.sprite_texture(),
                    level.camera()
                );

            level.finish_render();
        }
    );

    pixel::app().run();
}


void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{320, 224};
    glm::ivec2 actual_window_size = virtual_window_size * 2;

    pixel::init(actual_window_size, virtual_window_size, argc, argv);

    Level level{virtual_window_size};

    run(level);
}


int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}


