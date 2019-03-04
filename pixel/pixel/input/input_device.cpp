#include "input_device.h"

using namespace pixel::input;

bool InputDevice::is_button_pressed(int code)
{
    return false;
}

bool InputDevice::is_button_pressed_repeat(int code)
{
    return false;
}

bool InputDevice::is_button_released(int code)
{
    return false;
}

bool InputDevice::is_button_down(int code)
{
    return false;
}

float InputDevice::get_axis(int axis)
{
    return 0;
}

size_t InputDevice::num_axes()
{
    return 0;
}

void InputDevice::on_button_released(int code)
{

}

void InputDevice::on_button_pressed(int code)
{

}

size_t InputDevice::num_buttons()
{
    return 0;
}
