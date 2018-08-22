#include <limits>
#include <algorithm>
#include <pixel/pixel.h>
#include <pixel/level.h>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;

float PAN_SPEED = 3.0f;

void run(Level& level, const string& map_path)
{
    level.load_map(map_path);

    app().set_background_color(level.tile_map().background_color());

    //level.camera().follow(guy.position);

    //glClearColor(0.1, 0.1, 0.1, 1);

    pixel::app().set_tick_callback(
        [&] {

            glm::vec2 d{0};

            if (input::Keyboard::keymap[GLFW_KEY_RIGHT]) {
                d.x += PAN_SPEED;
            } else if (input::Keyboard::keymap[GLFW_KEY_LEFT]) {
                d.x -= PAN_SPEED;
            }

            if (input::Keyboard::keymap[GLFW_KEY_UP]) {
                d.y += PAN_SPEED;
            } else if (input::Keyboard::keymap[GLFW_KEY_DOWN]) {
                d.y -= PAN_SPEED;
            }

            level.camera().translate(d);

            level.begin_render();

            level.renderer_group().get<renderers::TileMapRenderer>().render(level.tile_map(), level.camera());

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

    if (argc > 0) {
        string map_path(argv[0]);

        Level level{virtual_window_size};

        run(level, map_path);
    }
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
