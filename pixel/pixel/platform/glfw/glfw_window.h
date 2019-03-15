#ifndef PIXEL_MAIN_GLFW_WINDOW_H
#define PIXEL_MAIN_GLFW_WINDOW_H

#include <pixel/platform/window.h>
#include <string>

struct GLFWwindow;

namespace pixel
{

using namespace std;

class GlfwWindow : public Window
{
public:
    GlfwWindow(int window_width, int window_height, const string& name);

    virtual void* get_native_handle() override;
    
private:
    GLFWwindow* native_window_;
};

};
#endif //PIXEL_MAIN_GLFW_WINDOW_H
