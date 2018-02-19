#include <chrono>
#include <thread>

#include <pixel/graphics/common.h>
#include <pixel/util/util.h>
#include <pixel/error.h>

#include "frame_rate_limiter.h"

using namespace pixel;
using namespace pixel::time;
using namespace std;


void FrameRateLimiter::delay(const double used) const
{
    using seconds = chrono::duration<double, std::ratio<1>>;

    double elapsed_time = used;

    while (elapsed_time < frame_rate_ && (frame_rate_ - elapsed_time) > error_) {
        double sleep_start = glfwGetTime();

        this_thread::sleep_for(seconds(max((frame_rate_ - elapsed_time) / 2 - error_, error_ / 2)));

        elapsed_time += glfwGetTime() - sleep_start;
    }
}


FrameRateLimiter::FrameRateLimiter(const double frame_rate, const double error)
{
    argument_error_unless(frame_rate > 0, "frame_rate must be > 0");
    argument_error_unless(error > 0, "error must be > 0");

    frame_rate_ = frame_rate;
    error_ = error;
}
