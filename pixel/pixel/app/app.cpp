#include "app.h"
#include <pixel/time/frame_rate_limiter.h>
#include <pixel/util/util.h>

using namespace pixel;
using namespace std;

void glfw_error_callback(int err, const char *description) {
    cerr << "GLFW error encountered: " << err << endl;
    cerr << "Error description from glfw:" << endl;
    cerr << description << endl << endl;
}

glm::ivec2 window_size(GLFWwindow* window) {
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

    window_ = glfwCreateWindow(640, 448, "pixel", nullptr, nullptr);

    error_unless(window_, "glfwCreateWindow failed!");

    glfwMakeContextCurrent(window_);

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

    update_render_context();
}

void App::update_render_context() {
   render_context_.window_size = window_size(window_);
}

void App::run()
{
    glClearColor(0.1, 0.1, 0.1, 1);

    while (!glfwWindowShouldClose(window_)) {
        tick();

        render_context_.update_viewport();
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        if (tick_callback_) {
            tick_callback_();
        }

        glfwSwapBuffers(window_);

        lateTick();
    }
}

void App::setTickCallback(std::function<void(void)> cb) {
    tick_callback_ = std::move(cb);
}

void App::tick()
{
    fps_counter_.tick();
    ++frames_;

    #if PIXEL_DEBUG
        if (frames_ % 60 == 0) {
            cout << fps_counter_.fps() << " FPS" << endl;
        }
    #endif
}

void App::lateTick()
{
    static pixel::time::FrameRateLimiter limiter(1/60.0, (1/60.0) / 1000);

    limiter.delay(fps_counter_.timeSinceFrameStart());
}

RenderContext App::render_context() const
{
    return render_context_;
}
