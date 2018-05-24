#include "system.h"
#include <pixel/input/input.h>

void pixel::init()
{
   init(DEFAULT_WINDOW_SIZE * (int) DEFAULT_PIXEL_SCALE, DEFAULT_WINDOW_SIZE);
}

void pixel::init(glm::ivec2 actual_window_size, glm::ivec2 virtual_window_size)
{
    pixel::print_version_information();

    pixel::set_app(
        make_unique<App>(
            actual_window_size,
            virtual_window_size
        ).release()
    );

    input::Keyboard::register_callback(app().window());
}
