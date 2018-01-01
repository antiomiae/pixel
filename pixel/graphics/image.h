#ifndef GRAPHICS_IMAGE_H
#define GRAPHICS_IMAGE_H

#include <string>
#include <cstdint>

namespace pixel {
namespace graphics {

struct ImageData {
    int width, height, bpp;
    uint8_t *data;

    ImageData(int width, int height, int bpp = 4);

    ImageData(int width, int height, int bpp, uint8_t *data);

    ImageData(ImageData &&other) noexcept;

    ~ImageData();

    size_t length();
};

ImageData load_png(std::string);

}
};

#endif //GRAPHICS_IMAGE_H
