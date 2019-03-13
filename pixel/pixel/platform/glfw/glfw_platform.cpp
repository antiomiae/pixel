#include <string>
#include <iostream>
#include <unistd.h>

#include "glfw_platform.h"
#include "glfw_window.h"
#include <pixel/error.h>
#include <pixel/time/frame_rate_limiter.h>
#include <pixel/input/input.h>

#define GLFW_INCLUDE_NONE

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace pixel;


void glfw_error_callback(int err, const char* description)
{
    cerr << "GLFW error encountered: " << err << endl;
    cerr << "Error description from glfw:" << endl;
    cerr << description << endl << endl;
}

glm::ivec2 window_size(GLFWwindow* window)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    return {w, h};
};

void GlfwPlatform::init()
{
    if (!glfwInit()) {
        cout << "glwInit failed!" << endl;
        throw;
    }

    glfwSetErrorCallback(glfw_error_callback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(render_context_.window_size.x, render_context_.window_size.y, "pixel", nullptr, nullptr);

    error_unless(window_, "glfwCreateWindow failed!");

    glfwMakeContextCurrent(window_);

    glfwSwapInterval(1);

    auto err = glewInit();

    error_unless(err == GLEW_OK, string("glewInit failed!") + reinterpret_cast<const char*>(glewGetErrorString(err)));
    error_unless(GLEW_VERSION_4_1, "OpenGL version 4.1 is not available according to GLEW");

}

Window* GlfwPlatform::get_window()
{
    return static_cast<Window*>(window_.get());
}

void GlfwPlatform::make_context_current()
{
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(window_->get_native_handle()));
}
