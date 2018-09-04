

#ifndef PIXEL_MAIN_AUDIO_CONTROLLER_H
#define PIXEL_MAIN_AUDIO_CONTROLLER_H

#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include <unordered_map>
#include <string>

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

    void set_background_music(const std::string& path)
    {
        auto& source_ptr = source_cache_[path];

        if (!source_ptr) {
            source_ptr = std::make_unique<SoLoud::WavStream>();
            static_cast<SoLoud::WavStream&>(*source_ptr).load(path.c_str());
            source_ptr->setLooping(1);
        }

        engine_.stop(background_music_voice_handle_);

        background_music_voice_handle_ = engine_.play(*source_ptr);

        engine_.setProtectVoice(background_music_voice_handle_, true);
    }

    ~AudioController()
    {
        engine_.deinit();
    }

private:
    SoLoud::Soloud engine_{};
    std::unordered_map<std::string, std::unique_ptr<SoLoud::AudioSource>> source_cache_{};
    SoLoud::handle background_music_voice_handle_;
};

};

#endif //PIXEL_MAIN_AUDIO_CONTROLLER_H
