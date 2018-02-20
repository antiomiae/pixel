//
//

#ifndef PIXEL_SETUP_H
#define PIXEL_SETUP_H

#include <pixel/pixel.h>
#include <gtest/gtest.h>

namespace pixeltest
{

pixel::App& app();

void setup();
void teardown();

}

#endif //PIXEL_SETUP_H
