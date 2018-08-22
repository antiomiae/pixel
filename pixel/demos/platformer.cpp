#include <limits>
#include <algorithm>
#include <pixel/pixel.h>
#include <pixel/level.h>
#include <pixel/tilemap/tile_map_collider.h>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;

struct Guy
{

    glm::vec2 position{0.0f};
    glm::vec2 size{0.0f};
    glm::vec2 velocity{0.0f};
    glm::vec2 acc{1.f, 1.f};

    float ground_friction = 0;
    float air_friction = 0;

    float max_run_speed = 2.5f;
    float max_jump_speed = 5.0f;
    float min_jump_speed = 3.0f;

    bool in_air{false};
    bool jumping{false};
    bool can_jump{false};

    int layer{0};
    Level* level;

    Sprite sprite;

    Guy(Level* level, const string& sprite_name)
        : level{level}
    {
        sprite = level->get_sprite(sprite_name);
        size = {sprite.texture_region.w, sprite.texture_region.h};
    }

    void walk_movement(float dt)
    {
        if (input::Keyboard::keymap[GLFW_KEY_SPACE] && can_jump) {
            jumping = true;
            in_air = true;
            velocity.y = max_jump_speed;
            can_jump = false;
        }

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

        velocity.x = glm::clamp(velocity.x, -max_run_speed, max_run_speed);

        auto collision_axes = move(dt);

        if (collision_axes.x != 0) {
            velocity.x = 0;
        }

        if (collision_axes.y != 0) {
            velocity.y = 0;

            // we hit ground
            if (collision_axes.y == -1) {
                in_air = false;
                jumping = false;
            }
        }
    }

    glm::ivec2 move(float dt) {
        CollisionRect rect(
            position + size/2.0f,
            size/2.0f,
            velocity,
            {16, 16}
            );

        auto [collision_axes, center] = TileMapCollider().collide(
            rect,
            level->tile_map().layers()[layer],
            [&](auto& tile, auto& tile_desc) {
                auto type = tile_desc.type;
                return type == "brick";
            },
            true
        );

        position = center - size/2.0f;

        if (overlapping_solid_tiles()) {
            cout << "Error condition: overlapping solid tiles after collision routine" << endl;
            cout << rect.debug_print() << endl;
        }

        return collision_axes;
    }

    void update(float dt)
    {
        if (!input::Keyboard::keymap[GLFW_KEY_SPACE]) {
            can_jump = true;
        }

        if (!in_air && !jumping) {
            if (on_ground()) {
                walk_movement(dt);
            } else {
                in_air = true;
            }
        }

        if (in_air || jumping) {
            jump_movement(dt);
        }

        move(dt);

        level->camera().follow(position);
    }

    void jump_movement(float dt)
    {
        if (jumping) {
            // check for jump key down
            if (!input::Keyboard::keymap[GLFW_KEY_SPACE]) {
                jumping = false;
                velocity.y = glm::min(velocity.y, min_jump_speed);
            }
        } else {

        }

        if (velocity.y <= 0) {
            jumping = false;
        }

        if (input::Keyboard::keymap[GLFW_KEY_RIGHT]) {
            velocity.x += acc.x;
        } else if (input::Keyboard::keymap[GLFW_KEY_LEFT]) {
            velocity.x -= acc.x;
        }  else {
            if (velocity.x != 0) {
                velocity.x += acc.x * (velocity.x > 0 ? -1 : 1);
            }

            if (abs(velocity.x) < acc.s) {
                velocity.x = 0;
            }
        }

        velocity.x = glm::clamp(velocity.x, -max_run_speed, max_run_speed);

        velocity.y += acc.y;

        auto collision_axes = move(dt);

        if (collision_axes.x != 0) {
            velocity.x = 0;
        }

        if (collision_axes.y != 0) {
            // try to prevent sticking
            velocity.y = 0;

            // we hit ground
            if (collision_axes.y == -1 && on_ground()) {
                velocity.y = 0;
                in_air = false;
                jumping = false;
            }
        }


    }

    bool on_ground()
    {
        auto& tile_layer = level->tile_map().layers()[layer];
        auto tile_size = level->tile_map().tile_size();

        bool found_solid_tile = false;

        tile_layer.visit_tiles(
            {
                (int) floor(position.x) / (int)tile_size.x,
                (int) floor(position.y) / (int)tile_size.y
            },
            {
                (int) ceil(position.x + size.x - 1) / (int)tile_size.x,
                (int) ceil(position.y - 1) / (int)tile_size.y
            },
            [&] (auto tile_coord, auto& tile) -> bool {
                if (tile.tile_id != 0) {
                    auto& tile_desc = tile_layer.parent().tileset().tile(tile.tile_id);

                    if (tile_desc.type == "brick") {
                        found_solid_tile = true;
                        return false;
                    }
                }

                return true;
            }
        );

        return found_solid_tile;
    }

    void render(SpriteBatch& batch)
    {
        sprite.position = position;
        batch.add(sprite);
    }

    bool overlapping_solid_tiles()
    {
        auto& tile_layer = level->tile_map().layers()[layer];
        auto tile_size = level->tile_map().tile_size();
        auto tile_count = tile_layer.parent().tile_count();
        auto extents = tile_count * tile_size;

        bool found_solid_tile = false;
        int minx = max(position.x, 0.0f);
        int miny = max(position.y, 0.0f);
        int maxx = min((int)(position.x + size.x - 1), (int)(tile_count.x * tile_size.x - 1));
        int maxy = min((int)(position.y + size.y - 1), (int)(tile_count.y * tile_size.y - 1));

        if (maxy < 0 || maxx < 0 || minx > (extents.x - 1) || miny > (extents.y - 1)) {
            return false;
        }

        tile_layer.visit_tiles(
            {
                max(minx / (int)tile_size.x, 0),
                max(miny / (int)tile_size.y, 0)
            },
            {
                min(maxx / (int)tile_size.x, (int)tile_count.x - 1),
                min(maxy / (int)tile_size.y, (int)tile_count.y - 1)
            },

            [&] (auto tile_coord, auto& tile) -> bool {
                if (tile.tile_id != 0) {
                    auto& tile_desc = tile_layer.parent().tileset().tile(tile.tile_id);

                    if (tile_desc.type == "brick") {
                        found_solid_tile = true;
                        return false;
                    }
                }

                return true;
            }
        );

        return found_solid_tile;
    }

};


void run(Level& level)
{
    level.load_sprites({"assets/guy.png"});
    level.load_map("assets/map2.tmx");

    Guy guy{&level, "assets/guy.png"};
    guy.layer = 0;
    guy.acc = {0.25f, -0.25f};
    guy.position = {
        16 * 3,
        16 * 10
    };


    /*
     * position
x = {float} 65.25
y = {float} 69

velocity
x = {float} 63.25
y = {float} 73.75
     */


    SpriteBatch batch;

    level.camera().follow(guy.position);

    glClearColor(0.1, 0.1, 0.1, 1);

    pixel::app().set_tick_callback(
        [&] {
            guy.update(1 / 60.0f);

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

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    Level level{virtual_window_size};

    run(level);
}


int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}


/*
 *
position
x = {float} 371.5
y = {float} 20.9999981

velocity
x = {float} 5
y = {float} -5

 */
