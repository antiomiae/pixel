#include "image.h"

#include <tinypng/png.h>
#include "../util/util.h"

using namespace pixel::graphics;
using namespace pixel::util;

ImageData pixel::graphics::load_png(std::string path)
{
    if (!file_exists(path)) {
        error("file does not exist at path: " + path);
    }
    tinypng::PNG png(path);

    ImageData out(png.getWidth(), png.getHeight());

    memcpy(out.data, png.buffer(), out.length());

    return out;
};


ImageData::ImageData(int width, int height, int bpp)
        : ImageData(width, height, bpp, nullptr)
{
    data = new uint8_t[width * height * bpp];
};


ImageData::ImageData(int width, int height, int bpp, uint8_t *data)
        : width(width), height(height), bpp(bpp), data(data)
{

};


ImageData::ImageData(ImageData && o) noexcept
        : width(o.width), height(o.height), bpp(o.bpp)
{
    data = o.data;
    o.data = nullptr;
};


ImageData::~ImageData()
{
    if (data) {
        delete data;
        data = nullptr;
    }
}


size_t ImageData::length()
{
    return static_cast<size_t>(width * height * bpp);
};
