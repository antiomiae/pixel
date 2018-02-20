#include "image.h"

#include <tinypng/png.h>
#include "../util/util.h"

using namespace pixel::graphics;
using namespace pixel::util;


ImageData pixel::graphics::load_png(const string& path)
{
    if (!file_exists(path)) {
        pixel_error("file does not exist at path: " + path);
    }

    tinypng::PNG png(path);

    ImageData out(png.getWidth(), png.getHeight());

    memcpy(out.data, png.buffer(), out.length());

    return out;
};


bool pixel::graphics::save_png(const ImageData& img, const std::string& path)
{
    tinypng::PNG png(img.width, img.height, img.data);

    return png.writeToFile(path);
}


ImageData::ImageData(const std::string& path)
  : ImageData{load_png(path)}
{
};


ImageData::ImageData(unsigned int _width, unsigned int _height)
  : ImageData(_width, _height, nullptr)
{
    data = new uint8_t[width * height * bpp];
    external_data = false;
};


ImageData::ImageData(unsigned int _width, unsigned int _height, uint8_t* _data)
  : width(_width), height(_height), data(_data), external_data(_data != nullptr)
{
};


ImageData::ImageData(ImageData&& o) noexcept
  : width(o.width), height(o.height), external_data(o.external_data)
{
    data = o.data;
    o.data = nullptr;
};


ImageData::~ImageData()
{
    if (data && !external_data) {
        delete[] data;
        data = nullptr;
    }
}


size_t ImageData::length()
{
    return size_t{(width * height * bpp)};
}


ImageData ImageData::subregion(unsigned int x0, unsigned int y0, unsigned int _width, unsigned int _height)
{
    ImageData temp(_width, _height);

    for (auto y_ = 0; y_ < _height; ++y_) {
        for (auto x_ = 0; x_ < _width; ++x_) {
            auto local_pixel = (y_ + y0) * width + x_ + x0;
            auto temp_pixel = y_ * _width + x_;

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
