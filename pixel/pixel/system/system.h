//
//

#ifndef PIXEL_SYSTEM_H
#define PIXEL_SYSTEM_H

#include "version.h"
#include <pixel/math.h>
#include <pixel/app/app.h>

namespace pixel
{

void init();

void init(
    glm::ivec2 actual_window_size, glm::ivec2 virtual_window_size,
    int* argc = nullptr, char** argv = nullptr
);

}

#endif //PIXEL_SYSTEM_H
