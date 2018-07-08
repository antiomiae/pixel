//
//

#include <valarray>
#include <algorithm>
#include <iostream>
#include "frame_counter.h"
#include <GLFW/glfw3.h>

float pixel::time::FrameCounter::fps() const
{
    return _fps;
}

double pixel::time::FrameCounter::timeSinceFrameStart() const {
    return glfwGetTime() - _last_tick;
}

void pixel::time::FrameCounter::tick()
{
    static std::valarray<double> times(60);
    static int index = 0;

    if (_last_tick == 0) {
        _last_tick = glfwGetTime();
    }

    double current_time = glfwGetTime();

    times[index] = (current_time - _last_tick);

    ++index;

    if (index >= times.size()) {
        index = 0;
    }

    //_time += current_time - _last_tick;

    ++_frames;

    if (_frames % 60 == 0) {
        _fps = _frames / times.sum();
        _time = 0;
        _frames = 0;
    }

    _last_tick = current_time;
}
