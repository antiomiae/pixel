#include <functional>
#include "input.h"
#include <imgui.h>
#include <pixel/imgui/imgui_impl_glfw.h>

namespace pixel::input {

Keyboard::Keymap Keyboard::keymap = Keyboard::Keymap();

void Keyboard::clear_keymap()
{
    for (auto i = 0u; i <= GLFW_KEY_LAST; ++i) {
        keymap[i] = false;
    }
}

void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    if (!action || !ImGui::GetIO().WantCaptureKeyboard) {
        keymap[key] = action;
    }
}

void Keyboard::register_callback(GLFWwindow* window)
{
    clear_keymap();
    glfwSetKeyCallback(window, Keyboard::key_callback);
}

}


