//
//

#ifndef PIXEL_SETUP_H
#define PIXEL_SETUP_H

#include <pixel/pixel.h>

namespace pixeltest
{

pixel::App *_app = nullptr;

pixel::App &app();

pixel::App &app()
{
    if (_app == nullptr) {
        throw "must call setup() to initialize pixel::App instance!";
    }

    return *_app;
}

void setup()
{
    _app = new pixel::App();

    _app->init();
}

void teardown() {

}

}

#endif //PIXEL_SETUP_H
