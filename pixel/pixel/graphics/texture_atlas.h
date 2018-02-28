

#ifndef PIXEL_TEXTURE_ATLAS_H
#define PIXEL_TEXTURE_ATLAS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <pixel/math/math.h>
#include "texture_region.h"
#include "image.h"

namespace pixel::graphics
{
using namespace std;

const unsigned kDefaultTextureAtlasTextureWidth = 256;
const unsigned kDefaultTextureAtlasTextureHeight = 256;
const unsigned kDefaultTextureAtlasTextureLayers = 10;

class TextureAtlas
{
public:
    TextureAtlas() = default;
    explicit TextureAtlas(glm::uvec3 size);
    void start_batch();
    void stop_batch();
    uint32_t add_image(const string& path);

private:
    struct _ImageSize
    {
        unsigned w, h;
        uint32_t region_id;


        _ImageSize(unsigned w, unsigned h, uint32_t region_id)
            : w{w},
              h{h},
              region_id{region_id}
        { }
    };

    uint32_t top_id_{0};
    unordered_map<string, uint32_t> file_id_map_;
    unordered_map<uint32_t, TextureRegion> tex_regions_;
    vector<_ImageSize> blocks_;
    unordered_map<uint32_t, ImageData> image_buffers_;
    glm::uvec3 texture_dimensions_ = {
        kDefaultTextureAtlasTextureWidth,
        kDefaultTextureAtlasTextureHeight,
        kDefaultTextureAtlasTextureLayers
    };
};

};
#endif //PIXEL_TEXTURE_ATLAS_H
