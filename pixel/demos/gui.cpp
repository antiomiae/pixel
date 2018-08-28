#include <pixel/pixel.h>
#include <imgui.h>
#include <pixel/imgui/imgui_impl_glfw.h>
#include <pixel/imgui/imgui_impl_opengl3.h>

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
    ImGui_ImplGlfw_InitForOpenGL(pixel::app().window(), true);
    ImGui_ImplOpenGL3_Init(nullptr);
}

void gui()
{
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::ShowDemoWindow(&show_demo_window);
}

void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{1400, 800};
    glm::ivec2 actual_window_size = virtual_window_size * 1;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    setup_imgui();

    pixel::app().set_background_color({1, 1, 1, 1});

    pixel::app().set_tick_callback(
        [&] {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // doesn't use opengl at all
            gui();
            ImGui::Render();

            glClear(GL_COLOR_BUFFER_BIT);
            // now it do
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

