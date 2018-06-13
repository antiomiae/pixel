#include "system.h"
#include <memory>
#include <pixel/input/input.h>

using namespace std;

void pixel::init()
{
   init(DEFAULT_WINDOW_SIZE * (int) DEFAULT_PIXEL_SCALE, DEFAULT_WINDOW_SIZE);
}

void pixel::init(glm::ivec2 actual_window_size, glm::ivec2 virtual_window_size,
                 int argc, char** argv)
{
    pixel::print_version_information();

    pixel::set_app(
        make_unique<App>(
            actual_window_size,
            virtual_window_size
        ).release()
    );

    if (argc >= 2) {
        app().set_app_dir(string{argv[1]});
        app().change_to_app_dir();

        argv = &argv[2];
        argc -= 2;
    }

    input::Keyboard::register_callback(app().window());
}
