#ifndef PIXEL_MAIN_WINDOW_H
#define PIXEL_MAIN_WINDOW_H

namespace pixel
{

class Window
{
public:
    virtual ~Window() = default;

    virtual void* get_native_handle()
    { return nullptr; };
};

};


#endif //PIXEL_MAIN_WINDOW_H
