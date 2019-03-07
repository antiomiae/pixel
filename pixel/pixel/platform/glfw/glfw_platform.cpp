#include "glfw_platform.h"
#include "glfw_window.h"

#define GLFW_INCLUDE_NONE

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace pixel;

void GlfwPlatform::init()
{

}

Window* GlfwPlatform::get_window()
{
    return static_cast<Window*>(window_.get());
}

void GlfwPlatform::make_context_current()
{
    glfwMakeContextCurrent(window_->get_native_handle());
}
