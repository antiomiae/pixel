#include <string>
#include <iostream>
#include <unistd.h>

#include "app.h"
#include <pixel/error.h>
#include <pixel/time/frame_rate_limiter.h>
#include <pixel/imgui/gui.h>
#include <pixel/input/input.h>


namespace pixel
{

using namespace std;

void set_clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}


App* App::shared_app = nullptr;

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




    update_render_context();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    pixel::input::Keyboard::register_callback(window_);

    set_clear_color(render_context_.default_clear_color);

    imgui_setup(window_);

    audio_controller_.init();

    virtual_render_target_ = make_unique<graphics::OffscreenRenderTarget>();
    virtual_render_target_->set_window_size(render_context_.virtual_window_size);
}

void App::update_render_context()
{
    render_context_.window_size = window_size(window_);
}

void App::run()
{
    glClear(GL_COLOR_BUFFER_BIT);

    while (!glfwWindowShouldClose(window_)) {
        imgui_frame_start();

        tick();

        glfwMakeContextCurrent(window_);

        update_render_context();

        {
            auto ws = render_context_.window_size;
            glViewport(0, 0, ws.x, ws.y);
        }

        set_clear_color(render_context_.default_clear_color);

        glfwPollEvents();

        virtual_render_target_->activate();
        // clear virtual buffer
        glClear(GL_COLOR_BUFFER_BIT);

        if (tick_callback_) {
            tick_callback_();
        }

        virtual_render_target_->deactivate();
        // clear default/main buffer
        glClear(GL_COLOR_BUFFER_BIT);

        /* blit virtual window to actual window */
        virtual_render_target_->draw(
            glm::ivec4(
                0,
                0,
                render_context_.window_size.x,
                render_context_.window_size.y
            )
        );

        imgui_render();

        glfwMakeContextCurrent(window_);
        glfwSwapBuffers(window_);

        late_tick();
    }

    imgui_shutdown();
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

graphics::RenderContext& App::render_context()
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

AudioController& App::audio_controller()
{
    return audio_controller_;
}

App& app()
{
    return *App::shared_app;
}

void App::set_app(App* new_shared_app)
{
    App::shared_app = new_shared_app;
}

};
