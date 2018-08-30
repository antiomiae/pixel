#include <pixel/pixel.h>
#include <pixel/level.h>
//#include <pixel/input
#include <imgui.h>
#include <pixel/imgui/imgui_impl_glfw.h>
#include <pixel/imgui/imgui_impl_opengl3.h>

using namespace pixel;
using namespace pixel::graphics;


class SpriteDebugMenu
{
public:
    SpriteDebugMenu(Sprite* sprite)
    : sprite_(sprite)
    {
    }

    void update()
    {
        if (!is_active_ && should_activate()) {
             is_active_ = true;
        }

        render();
    }

private:
    bool is_active_ = true;
    Sprite* sprite_ = nullptr;

    bool should_activate()
    {
        auto v = input::Keyboard::keymap[GLFW_KEY_D];
        return v;
    }

    void render()
    {
        if (!is_active_) {
            return;
        }

        if (!ImGui::Begin("Sprite", &is_active_)) {
            ImGui::End();
            return;
        }

        ImGui::SliderFloat("x", &sprite_->position.x, 0, app().render_context().virtual_window_size.x);
        ImGui::VSliderFloat("y", {18.0f, 160.0f}, &sprite_->position.y, 0, app().render_context().virtual_window_size.y);

        ImGui::SliderFloat("angle", &sprite_->angle, 0, 2*M_PI);

        ImGui::End();
    }
};

void setup_imgui()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    //const char* glsl_version = "150";
    ImGui_ImplGlfw_InitForOpenGL(pixel::app().window(), false);
    ImGui_ImplOpenGL3_Init(nullptr);
}

void gui()
{
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::ShowDemoWindow(&show_demo_window);
}

void imgui_frame_start()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void update(Level& level)
{
    static Sprite sprite = level.get_sprite("assets/sonic.png");
    static SpriteDebugMenu debug_menu(&sprite);

    debug_menu.update();

    level.begin_render();

    SpriteBatch batch;

    batch.add(sprite);

    level.renderer_group().get<pixel::renderers::SpriteRenderer>().render(batch.sprites(), level.sprite_texture(), level.camera());
}

void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{1400, 800};
    glm::ivec2 actual_window_size = virtual_window_size * 1;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    Level level(app().render_context().virtual_window_size);

    level.load_sprites({"assets/sonic.png"});

    setup_imgui();

    pixel::app().set_background_color({1, 1, 1, 1});

    pixel::app().set_tick_callback(
        [&] {
            imgui_frame_start();

            update(level);

            imgui_render();
        }
    );

    pixel::app().run();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}

