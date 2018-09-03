#include <pixel/pixel.h>
#include "soloud.h"
#include "soloud_wav.h"

void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{1400, 800};
    glm::ivec2 actual_window_size = virtual_window_size * 1;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    SoLoud::Soloud gSoloud; // SoLoud engine
    SoLoud::Wav gWave;      // One wave file

    gSoloud.init();

    gWave.load("assets/1.wav");

    gSoloud.play(gWave);

    pixel::app().set_tick_callback(
        [&] {

        }
    );

    pixel::app().run();
    gSoloud.init();
}

int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}


