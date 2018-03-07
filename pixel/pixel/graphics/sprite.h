//
//

#ifndef PIXEL_SPRITE_H
#define PIXEL_SPRITE_H

#include "texture_region.h"

namespace pixel::graphics
{

class Sprite
{
public:
    union
    {
        float position[3];
        struct
        {
            float x, y, z;
        };
    };
    float center[2];
    float angle;
    TextureRegion texture_region;
};

};
#endif //PIXEL_SPRITE_H
