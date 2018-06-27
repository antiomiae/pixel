#ifndef PIXEL_MAIN_SIMPLE_TIMER_H
#define PIXEL_MAIN_SIMPLE_TIMER_H

#include <chrono>

namespace pixel::time
{

using float_seconds = std::chrono::duration<float, std::ratio<1>>;

class SimpleTimer
{
public:
    SimpleTimer() = default;

    void reset()
    {
        time_ = float_seconds::zero();
    };

    void update(float seconds)
    {
        time_ += float_seconds(seconds);
    };

    template<class Duration = float_seconds>
    void update(Duration d)
    {
        time_ += std::chrono::duration_cast<float_seconds>(d);
    }

    float_seconds time()
    {
        return time_;
    }

private:
    float_seconds time_;
};

};

#endif //PIXEL_MAIN_SIMPLE_TIMER_H
