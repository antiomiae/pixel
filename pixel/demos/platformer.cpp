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

    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 velocity;
    glm::vec2 acc{1.f, 1.f};

    float ground_friction = 0;
    float air_friction = 0;


    bool in_air{false};
    bool jumping{false};

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
        if (input::Keyboard::keymap[GLFW_KEY_SPACE]) {
            jumping = true;
            in_air = true;
            velocity.y = 5;
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

        velocity.x = glm::clamp(velocity.x, -5.0f, 5.0f);

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
                velocity.y = glm::clamp(velocity.y, -5.0f, 2.5f);
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
        }

        velocity.x = glm::clamp(velocity.x, -5.0f, 5.0f);

        velocity.y += acc.y;

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

    bool on_ground()
    {
        auto& tile_layer = level->tile_map().layers()[layer];

        bool found_solid_tile = false;

        tile_layer.visit_tiles(
            {
                (int) floor(position.x) / 16,
                (int) floor(position.y) / 16
            },
            {
                (int) ceil(position.x + size.x - 1) / 16,
                (int) ceil(position.y - 1) / 16
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
        auto tile_count = tile_layer.parent().tile_count();

        bool found_solid_tile = false;
        int minx = max(position.x, 0.0f);
        int miny = max(position.y, 0.0f);
        int maxx = min(position.x + size.x - 1, tile_count.x * 16.0f - 1);
        int maxy = min(position.y + size.y - 1, tile_count.y * 16.0f - 1);

        tile_layer.visit_tiles(
            {
                max(minx / 16, 0),
                max(miny / 16, 0)
            },
            {
                min(maxx / 16, (int)tile_count.x - 1),
                min(maxy / 16, (int)tile_count.y - 1)
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
    level.load_sprites({"assets/mark1.png"});
    level.load_map("assets/map2.tmx");

    Guy guy{&level, "assets/mark1.png"};
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
    glm::ivec2 actual_window_size = virtual_window_size * 3;

    pixel::init(actual_window_size, virtual_window_size, argc, argv);

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
