#ifndef APP_APP_H
#define APP_APP_H

#include <functional>
#include <pixel/math.h>
#include <pixel/time/frame_counter.h>
#include <pixel/graphics/graphics.h>
#include <pixel/audio/audio.h>

namespace pixel
{

const glm::ivec2 DEFAULT_WINDOW_SIZE = {640, 480};
const glm::vec4 DEFAULT_CLEAR_COLOR = {0.0, 0.0, 0.0, 1.0};
const int DEFAULT_PIXEL_SCALE = 2;

class App
{

public:
    static App* shared_app;
    static void set_app(App*);

    App();
    App(const glm::ivec2& window_size, const glm::ivec2& virtual_window_size);
    App(const glm::ivec2& window_size, const glm::ivec2& virtual_window_size, const glm::vec4& clear_color);

    void init(int flags = 0);

    void run();
    void tick();
    void late_tick();

    void set_tick_callback(std::function<void(void)> cb);

    void update_render_context();

    const string& app_dir();
    void set_app_dir(const string& path);
    void change_to_app_dir();
    void set_background_color(glm::vec4);

    GLFWwindow* window();
    RenderContext& render_context();
    AudioController& audio_controller();

    float current_fps();

private:

    string app_dir_;

    int frames_{};

    GLFWwindow* window_;

    std::function<void(void)> tick_callback_{};

    pixel::time::FrameCounter fps_counter_{};

    RenderContext render_context_{
        DEFAULT_WINDOW_SIZE,
        DEFAULT_PIXEL_SCALE * DEFAULT_WINDOW_SIZE,
        DEFAULT_CLEAR_COLOR
    };

    AudioController audio_controller_;
};

App& app();

};

#endif //APP_APP_H
