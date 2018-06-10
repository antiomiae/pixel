#ifndef PIXEL_KEYBOARD_H
#define PIXEL_KEYBOARD_H

#include <unordered_map>
#include <list>
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


template<class ...Args>
class CallbackStack
{
public:
    using callback_t = function<bool(Args... args)>;

    void push_callback(callback_t cb)
    {
        callbacks_.push_front(move(cb));
    }

    void pop_callback()
    {
        callbacks_.pop_front();
    }

    void clear()
    {
        callbacks_.clear();
    }

    bool empty()
    {
        return callbacks_.empty();
    }

    bool invoke(Args&&... args)
    {
        if (!callbacks_.empty()) {
            for (auto& callback : callbacks_) {
                auto res = callback(forward<Args>(args)...);

                if (res) {
                    return true;
                }
            }
        }

        return false;
    }

    bool invoke_head(Args&&... args)
    {
        if (!callbacks_.empty()) {
            return callbacks_.front()(forward<Args>(args)...);
        }

        return true;
    }

private:
    list<callback_t> callbacks_;
};

};


#endif //PIXEL_KEYBOARD_H
