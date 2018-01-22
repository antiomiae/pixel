//
//

#include "frame_rate_limiter.h"
#include <pixel/util/util.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <chrono>
#include <thread>

using namespace std;

void pixel::time::FrameRateLimiter::delay(const double used) const
{
    using seconds = chrono::duration<double,std::ratio<1>>;

    double elapsed_time = used;

    while (elapsed_time < _frame_rate && (_frame_rate - elapsed_time) > _error) {
        double sleep_start = glfwGetTime();

        this_thread::sleep_for(seconds(max((_frame_rate - elapsed_time) / 2 - _error, _error)));

        elapsed_time += glfwGetTime() - sleep_start;
    }
}

pixel::time::FrameRateLimiter::
FrameRateLimiter(const double frame_rate, const double error)
{
    error_unless(frame_rate > 0, "frame_rate must be > 0");
    error_unless(error > 0, "error must be > 0");

    _frame_rate = frame_rate;
    _error = error;
}
