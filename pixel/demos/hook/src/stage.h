#ifndef PIXEL_MAIN_STAGE_H
#define PIXEL_MAIN_STAGE_H

#include <pixel/level.h>

class Stage : public pixel::Level
{
public:
    virtual ~Stage() = default;

    /* Hook methods */
    virtual void on_load() {};
    virtual void on_enter() {};
    virtual void on_exit() {};
    virtual void update() {};
private:

};


#endif //PIXEL_MAIN_STAGE_H
