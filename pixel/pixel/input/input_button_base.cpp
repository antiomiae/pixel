#include "input_button_base.h"

using namespace pixel::input;

InputButtonBase::InputButtonBase(size_t num_buttons)
    : num_buttons_{num_buttons}
{
    pressed_.resize(num_buttons);
    repeat_.resize(num_buttons);
    released_.resize(num_buttons);
    down_.resize(num_buttons);
}

void InputButtonBase::on_button_pressed(int code)
{
    if (!down_[code]) {
        pressed_[code] = true;
        down_[code] = true;
    }
}

void InputButtonBase::on_button_released(int code)
{
    if (down_[code]) {
        released_[code] = true;
        down_[code] = false;
    }

}

bool InputButtonBase::is_button_down(int code)
{
    if (code >= 0 && code < num_buttons_) {
        return down_[code];
    }
    return false;
}

bool InputButtonBase::is_button_pressed(int code)
{
    if (code >= 0 && code < num_buttons_) {
        return pressed_[code];
    }
    return false;
}

bool InputButtonBase::is_button_released(int code)
{
    if (code >= 0 && code < num_buttons_) {
        return released_[code];
    }
    return false;
}

bool InputButtonBase::is_button_pressed_repeat(int code)
{
    if (code >= 0 && code < num_buttons_) {
        return repeat_[code];
    }
    return false;
}

size_t InputButtonBase::num_buttons()
{
    return num_buttons_;
}
