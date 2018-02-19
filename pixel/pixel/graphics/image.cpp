#include "image.h"

#include <tinypng/png.h>
#include "../util/util.h"

using namespace pixel::graphics;
using namespace pixel::util;


ImageData pixel::graphics::load_png(const string& path)
{
    if (!file_exists(path)) {
        error("file does not exist at path: " + path);
    }

    tinypng::PNG png(path);

    ImageData out(png.getWidth(), png.getHeight());

    memcpy(out.data, png.buffer(), out.length());

    return out;
};


bool pixel::graphics::save_png(const ImageData& img, const std::string& path)
{
    tinypng::PNG png(img.width_, img.height_, img.data);

    return png.writeToFile(path);
}


ImageData::ImageData(const std::string& path)
  : ImageData{load_png(path)}
{
};


ImageData::ImageData(unsigned int width, unsigned int height)
  : ImageData(width, height, nullptr)
{
    data = new uint8_t[width * height * bpp];
    _external_data = false;
};


ImageData::ImageData(unsigned int width, unsigned int height, uint8_t* data)
  : width_(width), height_(height), data(data), _external_data(data != nullptr)
{
};


ImageData::ImageData(ImageData&& o) noexcept
  : width_(o.width_), height_(o.height_), _external_data(o._external_data)
{
    data = o.data;
    o.data = nullptr;
};


ImageData::~ImageData()
{
    if (data && !_external_data) {
        delete[] data;
        data = nullptr;
    }
}


size_t ImageData::length()
{
    return size_t{(width_ * height_ * bpp)};
}


ImageData ImageData::subregion(unsigned int x0, unsigned int y0, unsigned int width, unsigned int height)
{
    ImageData temp(width, height);

    for (auto y_ = 0; y_ < height; ++y_) {
        for (auto x_ = 0; x_ < width; ++x_) {
            auto local_pixel = (y_ + y0) * width_ + x_ + x0;
            auto temp_pixel = y_ * width + x_;

            for (auto b = 0u; b < bpp; ++b) {
                temp.data[temp_pixel * bpp + b] = data[local_pixel * bpp + b];
            }
        }
    }

    return temp;
}


bool ImageData::save(const string& path)
{
    return save_png(*this, path);
}
