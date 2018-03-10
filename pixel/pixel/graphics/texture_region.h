//
//

#ifndef PIXEL_TEXTURE_REGION_H
#define PIXEL_TEXTURE_REGION_H

#include <cstdint>
#include <pixel/math/math.h>

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
    /**
     * Whether pixel data should be transposed when used
     */
    int flipped;

    TextureRegion() = default;

    TextureRegion(int x, int y, int w, int h, int layer, bool flipped)
    : x{x},
      y{y},
      w{w},
      h{h},
      layer{layer},
      flipped(flipped)
    {
    }
};

};


#endif //PIXEL_TEXTURE_REGION_H
