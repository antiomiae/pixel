//
//

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
    if (_last_tick == 0) {
        _last_tick = glfwGetTime();
    }

    double current_time = glfwGetTime();

    _time += current_time - _last_tick;
    ++_frames;

    if (_time >= 0.25) {
        _fps = static_cast<float>(_frames / _time);
        _time = 0;
        _frames = 0;
    }

    _last_tick = current_time;
}
