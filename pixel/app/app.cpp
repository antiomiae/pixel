//
// Created by Kevin Ward on 12/23/17.
//

#include "app.h"

using namespace pixel;
using namespace std;

App::App()
{

}

void App::init(int flags)
{
    if (!glfwInit()) {
        cout << "glwInit failed!" << endl;
        throw;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    _window = glfwCreateWindow(640, 448, "pixel", nullptr, nullptr);
    if (!_window) {
        cout << "glfwCreateWindow failed!" << endl;
        throw;
    }

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
}

void App::run()
{
    glClearColor(0.95, 0, 0, 1);

    while (!glfwWindowShouldClose(_window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        if (_tickCallback) {
            _tickCallback();
        }

        glfwSwapBuffers(_window);

    }
}

void App::setTickCallback(std::function<void(void)> cb) {
    _tickCallback = std::move(cb);
}
