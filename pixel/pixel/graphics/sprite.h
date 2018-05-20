#ifndef PIXEL_SPRITE_H
#define PIXEL_SPRITE_H

#include "texture_region.h"

namespace pixel::graphics
{

class Sprite
{
public:
    /**
        * Sprite orientation flags
        * */
    enum FlipFlag : uint8_t
    {
        kHorizontal = 0x8,
        kVertical = 0x4,
        kDiagonal = 0x2
    };

    union
    {
        glm::vec3 position{};
        struct
        {
            float x, y, z;
        };
    };

    glm::vec2 center;

    glm::vec4 color{1.0, 1.0, 1.0, 1.0};

    glm::vec4 tint{0.0, 0.0, 0.0, 0.0};

    float angle;

    uint32_t flip_flags;

    TextureRegion texture_region;

    void flip_h(bool);
    bool flip_h();

    void flip_v(bool);
    bool flip_v();

    void flip_d(bool);
    bool flip_d();
};

};
#endif //PIXEL_SPRITE_H
