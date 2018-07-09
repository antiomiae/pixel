//
//

#ifndef PIXEL_FPS_COUNTER_H
#define PIXEL_FPS_COUNTER_H

namespace pixel::time
{

class FrameCounter
{
    int _frames;
    float _fps;
    double _last_tick;
public:
    float fps() const;
    double timeSinceFrameStart() const;

    void tick();
};

};

#endif //PIXEL_FPS_COUNTER_H
