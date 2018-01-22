//
//

#ifndef PIXEL_FRAME_RATE_LIMITER_H
#define PIXEL_FRAME_RATE_LIMITER_H

namespace pixel::time
{

class FrameRateLimiter
{
    double _frame_rate;
    double _error;

public:
    FrameRateLimiter(const double frame_rate, const double error);
    FrameRateLimiter() = delete;
    void delay(const double used) const;
};

};

#endif //PIXEL_FRAME_RATE_LIMITER_H
