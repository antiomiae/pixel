

#ifndef PIXEL_MAIN_AUDIO_CONTROLLER_H
#define PIXEL_MAIN_AUDIO_CONTROLLER_H

#include <soloud.h>

namespace pixel
{

class AudioCache
{
public:
    AudioCache() = default;
private:

};

class AudioController
{
public:
    AudioController() = default;
    AudioController(const AudioController&) = delete;

    void init()
    {
        engine_.init();
    }

    SoLoud::handle play(SoLoud::AudioSource& source)
    {
        return engine_.play(source);
    }

    ~AudioController()
    {
        engine_.deinit();
    }

private:
    SoLoud::Soloud engine_{};
    AudioCache sample_cache_{};

};

};

#endif //PIXEL_MAIN_AUDIO_CONTROLLER_H
