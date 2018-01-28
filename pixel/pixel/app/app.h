#ifndef APP_APP_H
#define APP_APP_H

#include "../graphics/graphics.h"
#include <functional>
#include <pixel/time/frame_counter.h>

namespace pixel {

class App {
    pixel::time::FrameCounter _fps_counter;
    int _frames;
    int _window_width;
    int _window_height;

    void tick();
public:
    GLFWwindow * _window;
    std::function<void(void)> _tickCallback;

    App() = default;

    void init(int flags = 0);

    void run();

    void setTickCallback(std::function<void(void)> cb);

    void lateTick();

    void updateViewport();

    std::pair<int,int> windowSize() const;
};

};

#endif //APP_APP_H
