#include <pixel/pixel.h>
#include <unistd.h>
#include <algorithm>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/color_space.hpp>
#include <random>
#include <glm/gtx/fast_square_root.hpp>
#include <chrono>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;

void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{640, 400} * 2.0f;
    glm::ivec2 actual_window_size = virtual_window_size ;

    pixel::init(actual_window_size, virtual_window_size, argc, argv);

    OffscreenRenderTarget render_target{};

    render_target.set_window_size(virtual_window_size);

    pixel::app().set_tick_callback(
        [&] {

            glClear(GL_COLOR_BUFFER_BIT);

            render_target.activate();

            glClear(GL_COLOR_BUFFER_BIT);

            /* do rendering */

            /* end */

            render_target.deactivate();

            /* blit virtual window to actual window */
            render_target.draw(
                glm::ivec4(
                    0,
                    0,
                    pixel::app().render_context().window_size.x,
                    pixel::app().render_context().window_size.y
                )
            );
        }
    );

    pixel::app().run();

}


int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}


