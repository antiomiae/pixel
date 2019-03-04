#ifndef PIXEL_MAIN_BASE_INPUT_H
#define PIXEL_MAIN_BASE_INPUT_H

#include <memory>

namespace pixel::input
{

class InputDevice
{
public:
    ~InputDevice() = default;

    virtual size_t num_buttons();

    virtual bool is_button_pressed(int code);
    virtual bool is_button_pressed_repeat(int code);
    virtual bool is_button_released(int code);
    virtual bool is_button_down(int code);

    virtual float get_axis(int axis);
    virtual size_t num_axes();

    virtual void on_button_pressed(int code);
    virtual void on_button_released(int code);

private:

};

};

#endif //PIXEL_MAIN_BASE_INPUT_H
