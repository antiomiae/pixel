#include <functional>
#include "keyboard.h"

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
    keymap[key] = action;
}

void Keyboard::register_callback(GLFWwindow* window)
{
    clear_keymap();
    glfwSetKeyCallback(window, Keyboard::key_callback);
}

}


