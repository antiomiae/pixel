//
//

#ifndef PIXEL_TEXTURE_REGION_H
#define PIXEL_TEXTURE_REGION_H

#include <cstdint>

namespace pixel::graphics {

struct TextureRegion
{
    union {
        struct {
            int32_t x;
            int32_t y;
            int32_t w;
            int32_t h;
        };

        int32_t rect[4];
    };
    int32_t layer;
};

};


#endif //PIXEL_TEXTURE_REGION_H
