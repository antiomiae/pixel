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

    static void
    collide(BoundingBox& object, TileLayer& tile_layer, const function<bool(TileLayer::Tile&, Tileset::Tile&)>& tile_callback)
    {
        auto& parent = tile_layer.parent();
        glm::ivec2 tile_size = parent.tile_size();
        glm::ivec2 tile_count = parent.tile_count();

        auto delta = object.end_position - object.start_position;

        if (delta == glm::vec2(0, 0)) {
            return;
        } else {
            int j = 10;
        }

        auto delta_inv = 1.0f / delta;

        auto test_rect = CollisionRect(object.start_position, object.size);

        glm::vec2 dir = {
            delta.x > 0 ? 1 : (delta.x < 0 ? -1 : 0),
            delta.y > 0 ? 1 : (delta.y < 0 ? -1 : 0)
        };

        /* return early if object isn't moving */
        if (dir == glm::vec2{0, 0}) {
            return;
        }

        /*
         * Sweep rectangle through the map:
         *  1. solve for collision time between an axis of the object and the tile grid
         *  2. advance object along path for shortest time
         *  3. check tiles at grid line for solidity
         *  4.
         */

        float acc_t = 0;
        glm::ivec2 collision_index = {-1, -1};

        while (delta != glm::vec2{0, 0}) {
            float ct = std::numeric_limits<float>::infinity();
            bool check_column = false;
            bool check_row = false;

            if (delta.x != 0) {

                float edge = test_rect.position.x + (dir.x == 1 ? test_rect.size.x : 0);
                int idx = (int) (dir.x > 0 ? ceil(edge) : floor(edge)) / (int) tile_size.x + (dir.x == 1 ? 1 : 0);

                // we've already checked this gridline and decided to go through it
                if (collision_index.x == idx) {
                    collision_index.x += dir.x;
                } else {
                    collision_index.x = idx;
                }

                // ensure we're going to collide with the map
                if (collision_index.x >= (dir.x == 1 ? 0 : 1) && collision_index.x <= (dir.x == 1 ? tile_count.x - 1 : tile_count.x)) {
                    float grid_x = collision_index.x * tile_size.x - (dir.x == 1 ? 1 : 0);
                    float d = abs(grid_x - edge);
                    ct = d * delta_inv.x;
                    check_column = true;
                }
            }

            if (delta.y != 0) {
                float edge = test_rect.position.y + (dir.y == 1 ? test_rect.size.y: 0);
                int idx = (int) edge / (int) tile_size.y + (dir.y == 1 ? 1 : 0);

                if (collision_index.y == idx) {
                    collision_index.y += dir.y;
                } else {
                    collision_index.y = idx;
                }

                // ensure we're going to collide with the map
                if (collision_index.y >= (dir.y == 1 ? 0 : 1) && collision_index.y <= (dir.y == 1 ? tile_count.y - 1 : tile_count.y)) {
                    float yt = abs((collision_index.y * tile_size.y - (dir.y == 1 ? 1 : 0) - edge) * delta_inv.y);

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
                cout << "ct > 1, delta = " << glm::to_string(delta) << endl;
                test_rect.position += delta;
                delta = {0, 0};
                break;
            }

            if (ct <= 1) {
                int k = 10;
            }

            auto column = collision_index.x - (dir.x < 0 ? 1 : 0);
            auto row = collision_index.y - (dir.y < 0 ? 1 : 0);

            auto d = ct * delta;

            if (check_column) {
                if (dir.x > 0) {
                    test_rect.position.x = collision_index.x * tile_size.x - 1 - test_rect.size.x;
                } else {
                    test_rect.position.x = (collision_index.x + 1) * tile_size.x;
                }
            } else {
                test_rect.position.x += d.x;
            }

            if (check_row) {
                if (dir.y > 0) {
                    test_rect.position.y = collision_index.y * tile_size.y - 1 - test_rect.size.y;
                } else {
                    test_rect.position.y = collision_index.y * tile_size.y - test_rect.size.y;
                }
            } else {
                test_rect.position.y += d.y;
            }

            delta -= d;
            delta_inv = 1.0f / delta;

            if (check_column) {
                glm::ivec2 y_span = {
                    clamp(test_rect.position.y, 0, tile_count.x * tile_size.x),
                    clamp(test_rect.position.y + test_rect.size.y, 0, tile_count.x * tile_size.x)
                };

                glm::uvec2 row_range = {
                    max((int) test_rect.position.y / tile_size.y, 0),
                    min((int) (test_rect.position.y + test_rect.size.y) / tile_size.y, tile_count.y - 1)
                };

                error_if(column < 0, "column index less than 0");

                if (row_range != glm::uvec2{0, 0}) {
                    for (auto y = row_range.s; y <= row_range.t; ++y) {
                        auto& tile = tile_layer.at(column, y);

                        if (tile.tile_id != 0) {
                            auto& tile_desc = parent.tileset().tile(tile.tile_id);

                            if (tile_callback(tile, tile_desc)) {
                                delta = {0, 0};
                                break;
                            }
                        }
                    }
                }
            }

            if (check_row) {
                glm::uvec2 col_range = {
                    max((int) test_rect.position.x / tile_size.x, 0),
                    min((int) (test_rect.position.x + test_rect.size.x) / tile_size.x, tile_count.x - 1)
                };

                error_if(row < 0, "row index less than 0");

                if (col_range != glm::uvec2{0, 0}) {
                    for (auto x = col_range.s; x <= col_range.t; ++x) {
                        auto& tile = tile_layer.at(x, row);

                        if (tile.tile_id != 0) {
                            auto& tile_desc = parent.tileset().tile(tile.tile_id);

                            if (tile_callback(tile, tile_desc)) {
                                delta = {0, 0};
                                break;
                            }
                        }
                    }
                }
            }

            if (ct == 0) {
                int k = 0;
            }

            if (abs(delta.x) <= 0.0001f) {
                delta.x = 0;
            }

            if (abs(delta.y) <= 0.0001f) {
                delta.y = 0;
            }
        }

        object.end_position = test_rect.position;
    }

    static pair<TileLayer::Tile&, Tileset::Tile&> tile_pair(TileLayer& tile_layer, int x, int y)
    {
        auto& tile = tile_layer.at(x, y);
        auto& parent = tile_layer.parent();
        auto& tile_desc = parent.tileset().tile(tile.tile_id);

        // TODO: check this
        return {tile, tile_desc};
    };
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
        input::Keyboard::keymap[GLFW_KEY_RIGHT] = true;
        //input::Keyboard::keymap[GLFW_KEY_UP] = true;

        if (input::Keyboard::keymap[GLFW_KEY_RIGHT]) {
            velocity.x += acc.x * dt;
        } else if (input::Keyboard::keymap[GLFW_KEY_LEFT]) {
            velocity.x -= acc.x * dt;
        } else {
            if (velocity.x > 0 || velocity.x < 0) {
                velocity.x += dt * acc.x * (velocity.x > 0 ? -1 : 1);
            }

            if (abs(velocity.x) < acc.x * dt) {
                velocity.x = 0;
            }
        }

        velocity.x = clamp(velocity.x, -100.0f, 100.0f);

        if (input::Keyboard::keymap[GLFW_KEY_UP]) {
            velocity.y += acc.x * dt;
        } else if (input::Keyboard::keymap[GLFW_KEY_DOWN]) {
            velocity.y -= acc.x * dt;
        } else {
            if (velocity.y > 0 || velocity.y < 0) {
                velocity.y += dt * acc.x * (velocity.y > 0 ? -1 : 1);
            }

            if (abs(velocity.y) < acc.x * dt) {
                velocity.y = 0;
            }
        }

        velocity.y = clamp(velocity.y, -100.0f, 100.0f);


        BoundingBox b;
        b.start_position = position;
        b.end_position = position + dt * velocity;
        b.size = {sprite.texture_region.w, sprite.texture_region.h};

        TileMapCollider::collide(
            b,
            level->tile_map().layers()[layer],
            [&](auto& tile, auto& tile_desc) {
                auto type = tile_desc.type;
                return type == "brick";
            }
        );

        position = b.end_position;
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
    level.load_map("assets/map2.tmx");

    Guy guy{&level, "assets/guy.png"};
    guy.layer = 0;

    guy.acc = {200.f, 0.f};

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


