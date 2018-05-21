#ifndef APP_APP_H
#define APP_APP_H

#include <functional>
#include <pixel/math.h>
#include <pixel/time/frame_counter.h>
#include <pixel/graphics/graphics.h>
#include <pixel/graphics/offscreen_render_target.h>

namespace pixel
{

const glm::ivec2 DEFAULT_WINDOW_SIZE = {640, 480};
const glm::vec4 DEFAULT_CLEAR_COLOR = {0.0, 0.0, 0.0, 1.0};
const int DEFAULT_PIXEL_SCALE = 2;

class App
{

public:

    App();
    App(const glm::ivec2& window_size, const glm::ivec2& virtual_window_size);
    App(const glm::ivec2& window_size, const glm::ivec2& virtual_window_size, const glm::vec4& clear_color);

    void init(int flags = 0);

    void run();
    void tick();
    void late_tick();

    void set_tick_callback(std::function<void(void)> cb);

    void update_render_context();

    GLFWwindow* window();
    RenderContext& render_context();

private:

    int frames_{};

    GLFWwindow* window_;

    std::function<void(void)> tick_callback_{};

    pixel::time::FrameCounter fps_counter_{};

    RenderContext render_context_{
        DEFAULT_WINDOW_SIZE,
        DEFAULT_PIXEL_SCALE * DEFAULT_WINDOW_SIZE,
        DEFAULT_CLEAR_COLOR
    };
};

App& app();

void set_app(App*);

};

#endif //APP_APP_H
