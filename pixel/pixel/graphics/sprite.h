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
        glm::vec3 position{};
        struct
        {
            float x, y, z;
        };
    };
    glm::vec2 center;
    float angle;
    TextureRegion texture_region;
};

};
#endif //PIXEL_SPRITE_H
