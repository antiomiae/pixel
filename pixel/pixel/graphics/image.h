#ifndef GRAPHICS_IMAGE_H
#define GRAPHICS_IMAGE_H

#include <string>
#include <cstdint>

namespace pixel::graphics
{

struct ImageData
{
    unsigned _width, _height;
    uint8_t* data;
    bool _external_data;

    explicit ImageData(const std::string& path);

    ImageData(unsigned int width, unsigned int height);

    ImageData(unsigned int width, unsigned int height, uint8_t* data);

    ImageData(ImageData&& other) noexcept;

    ImageData(ImageData&) = delete;
    ImageData(const ImageData&) = delete;

    ImageData& operator=(const ImageData&) = delete;

    ~ImageData();

    size_t length();

    ImageData subregion(unsigned int x0, unsigned int y0, unsigned int width, unsigned int height);

    bool save(const std::string& path);

    static const int bpp = 4;
};

ImageData load_png(const std::string& path);

bool save_png(const ImageData& img, const std::string& path);

};

#endif //GRAPHICS_IMAGE_H
