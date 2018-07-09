#include <experimental/optional>

#include <pixel/pixel.h>
#include <pixel/level.h>
#include <pixel/input/input.h>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/fast_square_root.hpp>

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
    //enum class
    static void
    collide(BoundingBox& object, TileLayer& tile_layer, const function<bool(TileLayer::Tile&)>& tile_callback)
    {
        auto& parent = tile_layer.parent();
        auto tile_size = glm::vec2(parent.tile_size());
        auto tile_count = glm::vec2(parent.tile_count());

        auto delta = object.end_position - object.start_position;

        bool x_dir = delta.x > 0 ? 1 : (delta.x < 0 ? -1 : 0);
        bool y_dir = delta.y > 0 ? 1 : (delta.y < 0 ? -1 : 0);

        /* return early if object isn't moving */
        if (x_dir == 0 && y_dir == 0) {
            return;
        }

        /*
         * Sweep rectangle through the map:
         *  1. solve for collision time between an axis of the object and the tile grid
         *  2. advance object along path for shortest time
         *  3. check tiles at grid line for solidity
         *  4.
         */

        object.end_position = object.start_position;

        glm::vec2 leading_edge = object.start_position;

        if (x_dir > 0) {
            leading_edge.x += object.size.x;
        }

        if (y_dir > 0) {
            leading_edge.y += object.size.y;
        }

        while(delta.x != 0 && delta.y != 0)
        {
            float x_t = 0;
            float y_t = 0;

            // the x coordinate of the nearest vertical grid line in the direction the object is moving
            auto next_x = -1;
            // the y coordinate of the nearest horizontal grid line in the direction the object is moving
            auto next_y = -1;

            if (x_dir != 0) {
                next_x = (((int)floor(leading_edge.x)) / tile_size.x + (x_dir > 0 ? 1 : 0)) * tile_size.x;

                x_t = abs(next_x - leading_edge.x) / delta.x;
            }

            if (y_dir != 0) {
                next_y = (((int)floor(leading_edge.y)) / tile_size.y + (y_dir > 0 ? 1 : 0)) * tile_size.y;

                y_t = abs(next_y - leading_edge.y) / delta.y;
            }

            if (x_t < y_t && x_t != 0) {
                leading_edge.x = 
            }

            break;
        }
    }

    static pair<TileLayer::Tile&, Tileset::Tile&> tile_pair(TileLayer& tile_layer, int x, int y)
    {
        auto& tile = tile_layer.at(x, y);
        auto& parent = tile_layer.parent();
        auto& tile_desc = parent.tileset().tile(tile.tile_id);

        return make_pair(tile, tile_desc);
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
            [&](auto& tile) {
                return tile.tile_id > 0;
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
    level.load_map("assets/map.tmx");

    Guy guy{&level, "assets/guy.png"};

    guy.acc = {200.f, 0.f};

    SpriteBatch batch;

    pixel::app().set_tick_callback(
        [&] {
            guy.update(1/60.0f);

            level.begin_render();

            batch.restart();

            level.renderer_group().get<renderers::TileMapRenderer>().render(level.tile_map(), level.camera());

            guy.render(batch);

            level.renderer_group().get<renderers::SpriteRenderer>().render(batch.sprites(), level.sprite_texture(), level.camera());

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


