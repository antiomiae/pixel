#include "setup.h"

namespace pixeltest
{

static pixel::App* _app = nullptr;


pixel::App& app()
{
    if (_app == nullptr) {
        throw "must call setup() to initialize pixel::App instance!";
    }

    return *_app;
}


void setup()
{
    pixel::print_version_information();

    _app = new pixel::App();

    _app->init();
}


void teardown()
{

}

}
