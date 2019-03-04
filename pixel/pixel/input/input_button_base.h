#ifndef PIXEL_MAIN_INPUT_BUTTON_BASE_H
#define PIXEL_MAIN_INPUT_BUTTON_BASE_H

#include "input_device.h"
#include <vector>


namespace pixel::input
{

using namespace std;

class InputButtonBase : public InputDevice
{
public:
    explicit InputButtonBase(size_t num_buttons);

    virtual size_t num_buttons() override;

    virtual void on_button_pressed(int code) override;
    virtual void on_button_released(int code) override;

    virtual bool is_button_down(int code) override;
    virtual bool is_button_pressed(int code) override;
    virtual bool is_button_released(int code) override;
    virtual bool is_button_pressed_repeat(int code) override;

private:
    size_t num_buttons_;

    vector<bool> pressed_;
    vector<bool> released_;
    vector<bool> repeat_;
    vector<bool> down_;
};

};

#endif //PIXEL_MAIN_INPUT_BUTTON_BASE_H
