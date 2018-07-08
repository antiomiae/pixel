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


