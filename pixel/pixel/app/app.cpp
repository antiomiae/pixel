#include <string>
#include "app.h"
#include <pixel/error.h>
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

    window_ = glfwCreateWindow(render_context_.window_size.x, render_context_.window_size.y, "pixel", nullptr, nullptr);

    error_unless(window_, "glfwCreateWindow failed!");

    glfwMakeContextCurrent(window_);

    glfwSwapInterval(1);

    auto err = glewInit();

    error_unless(err == GLEW_OK, string("glewInit failed!") + reinterpret_cast<const char*>(glewGetErrorString(err)));
    error_unless(GLEW_VERSION_4_1, "OpenGL version 4.1 is not available according to GLEW");

    update_render_context();
}

void App::update_render_context() {
   render_context_.window_size = window_size(window_);
}

void clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void App::run()
{
    clear_color(render_context_.default_clear_color);

    while (!glfwWindowShouldClose(window_)) {
        tick();

        update_render_context();

        {
            auto ws = render_context_.window_size;
            glViewport(0, 0, ws.x, ws.y);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        if (tick_callback_) {
            tick_callback_();
        }

        glfwSwapBuffers(window_);

        late_tick();
    }
}

void App::set_tick_callback(std::function<void(void)> cb) {
    tick_callback_ = std::move(cb);
}

void App::tick()
{
    fps_counter_.tick();
    ++frames_;

    #if PIXEL_DEBUG
//        if (frames_ % 60 == 0) {
//            cout << fps_counter_.fps() << " FPS" << endl;
//        }
    #endif
}

void App::late_tick()
{
    static pixel::time::FrameRateLimiter limiter(1/60.0, (1/60.0) / 1000);

    limiter.delay(fps_counter_.timeSinceFrameStart());
}

RenderContext& App::render_context()
{
    return render_context_;
}


App::App(glm::ivec2 window_size, glm::vec4 clear_color, float pixel_scale)
    : render_context_{window_size, clear_color, pixel_scale}
{
}


GLFWwindow& App::window()
{
    return *window_;
}
