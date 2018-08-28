#include <string>
#include <iostream>
#include <unistd.h>

#include "app.h"
#include <pixel/error.h>
#include <pixel/time/frame_rate_limiter.h>

namespace pixel
{

using namespace std;

App* shared_app = nullptr;

void set_clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

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

App::App()
{
    init();
}


App::App(const glm::ivec2& window_size, const glm::ivec2& virtual_window_size)
    : App{window_size, virtual_window_size, DEFAULT_CLEAR_COLOR}
{
}

App::App(const glm::ivec2& window_size, const glm::ivec2& virtual_window_size, const glm::vec4& clear_color)
    : render_context_{window_size, virtual_window_size, clear_color}
{
    init();
}

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

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(render_context_.window_size.x, render_context_.window_size.y, "pixel", nullptr, nullptr);

    error_unless(window_, "glfwCreateWindow failed!");

    glfwMakeContextCurrent(window_);

    glfwSwapInterval(1);

    auto err = glewInit();

    error_unless(err == GLEW_OK, string("glewInit failed!") + reinterpret_cast<const char*>(glewGetErrorString(err)));
    error_unless(GLEW_VERSION_4_1, "OpenGL version 4.1 is not available according to GLEW");

    update_render_context();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    set_clear_color(render_context_.default_clear_color);
}

void App::update_render_context()
{
    render_context_.window_size = window_size(window_);
}

void App::run()
{
    glClear(GL_COLOR_BUFFER_BIT);

    while (!glfwWindowShouldClose(window_)) {
        tick();
        glfwMakeContextCurrent(window_);

        update_render_context();

        {
            auto ws = render_context_.window_size;
            glViewport(0, 0, ws.x, ws.y);
        }

        set_clear_color(render_context_.default_clear_color);

        glfwPollEvents();

        if (tick_callback_) {
            tick_callback_();
        }

        glfwMakeContextCurrent(window_);
        glfwSwapBuffers(window_);

        late_tick();
    }
}

void App::set_tick_callback(std::function<void(void)> cb)
{
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
    static pixel::time::FrameRateLimiter limiter(1 / 60.0, (1 / 60.0) / 1000);

    limiter.delay(fps_counter_.timeSinceFrameStart());
}

RenderContext& App::render_context()
{
    return render_context_;
}


GLFWwindow* App::window()
{
    return window_;
}

const string& App::app_dir()
{
    return app_dir_;
}

void App::set_app_dir(const string& path)
{
    app_dir_ = path;
}

void App::change_to_app_dir()
{
    cout << "Changing to app directory " << app_dir_ << endl;
    chdir(app_dir_.c_str());
}

float App::current_fps()
{
    return fps_counter_.fps();
}

void App::set_background_color(glm::vec4 bg)
{
    render_context_.default_clear_color = bg;
}

App& app()
{
    return *shared_app;
}

void set_app(App* new_shared_app)
{
    shared_app = new_shared_app;
}

};
