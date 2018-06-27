#include "setup.h"
#include <pixel/pixel.h>

namespace pixeltest
{

static pixel::App* _app = nullptr;

void setup()
{
    pixel::init();
    pixel::app().update_render_context();
}


void teardown()
{

}

}
