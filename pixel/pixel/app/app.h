#ifndef APP_APP_H
#define APP_APP_H

#include "../graphics/graphics.h"
#include <functional>
#include <pixel/time/frame_counter.h>

namespace pixel {

class App {
    pixel::time::FrameCounter fps_counter_;
    int frames_;
    RenderContext render_context_;

    void tick();
public:
    GLFWwindow * window_;
    std::function<void(void)> tick_callback_;

    App() = default;
    void init(int flags = 0);
    void run();
    void setTickCallback(std::function<void(void)> cb);
    void lateTick();
    void update_render_context();
    RenderContext render_context() const;
};

};

#endif //APP_APP_H
