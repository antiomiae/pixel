#include <pixel/pixel.h>
#include <pixel/level.h>
#include "src/golf.h"


using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;


void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{320, 240};
    glm::ivec2 actual_window_size = virtual_window_size * 2;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    OffscreenRenderTarget render_target{};
    render_target.set_window_size(virtual_window_size);

    Level level{render_target.window_size()};

    pixel::app().set_tick_callback(
        [&] {

            glClear(GL_COLOR_BUFFER_BIT);

            render_target.activate();

            glClear(GL_COLOR_BUFFER_BIT);

            render_target.deactivate();

            /* blit virtual window to actual window */
            render_target.draw(
                glm::ivec4(
                    0,
                    0,
                    app().render_context().window_size.x,
                    app().render_context().window_size.y
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
