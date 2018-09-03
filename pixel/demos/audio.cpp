#include <pixel/pixel.h>
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{1400, 800};
    glm::ivec2 actual_window_size = virtual_window_size * 1;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    SoLoud::WavStream gWave;      // One wave file

    gWave.load("assets/38.ogg");
    gWave.setLooping(1);

    pixel::app().audio_controller().play(gWave);

    pixel::app().set_tick_callback(
        [&] {

        }
    );

    pixel::app().run();
}

int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}


