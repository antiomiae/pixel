#ifndef PIXEL_MAIN_GLFW_PLATFORM_H
#define PIXEL_MAIN_GLFW_PLATFORM_H

#include <pixel/platform/platform.h>
#include <memory>

namespace pixel
{

using namespace std;

class GlfwWindow;

class GlfwPlatform : public Platform
{
public:
    void init(Configuration config) override;
    Window* get_window() override;

    void make_context_current() override;

private:
    unique_ptr<pixel::GlfwWindow> window_;
};

};

#endif //PIXEL_MAIN_GLFW_PLATFORM_H
