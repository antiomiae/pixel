#ifndef APP_APP_H
#define APP_APP_H


#include <functional>
#include <pixel/math.h>
#include <pixel/time/frame_counter.h>
#include <pixel/graphics/graphics.h>


namespace pixel
{

const glm::ivec2 DEFAULT_WINDOW_SIZE = {640, 480};
const glm::vec4 DEFAULT_CLEAR_COLOR = {0.0, 0.0, 0.0, 1.0};
const float DEFAULT_PIXEL_SCALE = 2.0;

class App
{
public:
    App() = default;
    App(glm::ivec2 window_size, glm::vec4 clear_color, float pixel_scale);
    void init(int flags = 0);
    void run();
    void set_tick_callback(std::function<void(void)> cb);
    void late_tick();
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
        DEFAULT_CLEAR_COLOR,
        DEFAULT_PIXEL_SCALE
    };

    void tick();
};

};

#endif //APP_APP_H
