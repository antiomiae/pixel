#ifndef PIXEL_KEYBOARD_H
#define PIXEL_KEYBOARD_H

#include <unordered_map>
#include <pixel/graphics/common.h>

namespace pixel::input
{

using namespace std;

class Keyboard
{
public:
    using Keymap = unordered_map<uint32_t, bool>;

    Keyboard() = default;

    static void register_callback(GLFWwindow* window);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void clear_keymap();

    static Keymap keymap;

private:

};

};


#endif //PIXEL_KEYBOARD_H
