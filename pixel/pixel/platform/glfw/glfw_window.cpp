#include "glfw_window.h"
#include "common.h"

using namespace pixel;

GlfwWindow::GlfwWindow(int window_width, int window_height, const std::string& name)
{
    native_window_ = glfwCreateWindow(window_width, window_height, name.c_str(), nullptr, nullptr);
}

void* GlfwWindow::get_native_handle()
{
    return static_cast<void*>(native_window_);
}
