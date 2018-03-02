

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

class TextureAtlas
{
public:
    const unsigned kDefaultWidth = 256;
    const unsigned kDefaultHeight = 256;
    const unsigned kDefaultLayers = 10;

    TextureAtlas() = default;

    explicit TextureAtlas(glm::uvec3 size);

    void start_batch();
    void stop_batch();

    uint32_t add_image(const string& path);
    vector<ImageData>& layers();
    string debug_print() const;

    using RegionMap = unordered_map<uint32_t, TextureRegion>;
    using RegionMapItem = pair<uint32_t, TextureRegion>;

private:
    struct ImageSize
    {
        unsigned w, h;
        uint32_t region_id;

        ImageSize(unsigned w, unsigned h, uint32_t region_id)
            : w{w},
              h{h},
              region_id{region_id}
        {
        }
    };

    uint32_t top_id_{0};
    unordered_map<string, uint32_t> file_id_map_{};
    RegionMap tex_regions_{};
    vector<ImageData> layers_{};
    vector<ImageSize> blocks_{};
    unordered_map<uint32_t, ImageData> image_buffers_{};

    glm::uvec3 tex_size_ = {
        kDefaultWidth,
        kDefaultHeight,
        kDefaultLayers
    };

    void process_batch();
};

};
#endif //PIXEL_TEXTURE_ATLAS_H
