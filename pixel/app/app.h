#ifndef APP_APP_H
#define APP_APP_H

#include "../graphics/graphics.h"
#include <functional>

namespace pixel {

class App {
public:
    GLFWwindow * _window;
    std::function<void(void)> _tickCallback;

    App() = default;

    void init(int flags = 0);

    void run();

    void setTickCallback(std::function<void(void)> cb);
};

};

#endif //APP_APP_H
