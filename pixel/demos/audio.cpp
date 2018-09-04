#include <pixel/pixel.h>
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{1400, 800};
    glm::ivec2 actual_window_size = virtual_window_size * 1;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    pixel::app().audio_controller().set_background_music("assets/38.ogg");

    float time = 0;
    pixel::app().set_tick_callback(
        [&] {
            time += 1/60.0f;
        }
    );

    pixel::app().run();
}

int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}


