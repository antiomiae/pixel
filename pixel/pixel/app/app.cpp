//
// Created by Kevin Ward on 12/23/17.
//

#include "app.h"
#include <pixel/time/frame_rate_limiter.h>
#include <pixel/util/util.h>
#include <iostream>
#include <tuple>

using namespace pixel;
using namespace std;

void glfw_error_callback(int err, const char *description) {
    cerr << "GLFW error encountered: " << err << endl;
    cerr << "Error description from glfw:" << endl;
    cerr << description << endl << endl;
}

std::pair<int,int> windowDimensions(GLFWwindow *window) {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    return {w, h};
};

void App::init(int flags)
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

    _window = glfwCreateWindow(640, 448, "pixel", nullptr, nullptr);

    error_unless(_window, "glfwCreateWindow failed!");

    glfwMakeContextCurrent(_window);

    glfwSwapInterval(1);

    auto err = glewInit();
    if (err != GLEW_OK) {
        cout << "glewInit failed!" << endl;
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        throw;
    }

    if (!GLEW_VERSION_4_1)
    {
        cout << "OpenGL version 4.1 is not available according to GLEW" << endl;
        throw;
    }

    updateViewport();
}

void App::updateViewport() {
    int w, h;
    std::tie(w, h) = windowDimensions(_window);

    if (_window_width != w || _window_height != h) {
        glViewport(0, 0, w, h);
        _window_width = w;
        _window_height = h;
    }
}

void App::run()
{
    glClearColor(0.95, 0, 0, 1);

    while (!glfwWindowShouldClose(_window)) {
        tick();

        updateViewport();
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        if (_tickCallback) {
            _tickCallback();
        }

        glfwSwapBuffers(_window);

        lateTick();
    }
}

void App::setTickCallback(std::function<void(void)> cb) {
    _tickCallback = std::move(cb);
}

void App::tick()
{
    _fps_counter.tick();
    ++_frames;

    #if PIXEL_DEBUG
        if (_frames % 10 == 0) {
            cout << _fps_counter.fps() << " FPS" << endl;
        }
    #endif
}

void App::lateTick()
{
    pixel::time::FrameRateLimiter limiter(1/60.0, (1/60.0) / 1000);

    limiter.delay(_fps_counter.timeSinceFrameStart());
}

std::pair<int, int> App::windowSize() const
{
    return {_window_width, _window_height};
}
