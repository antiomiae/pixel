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
    tinypng::PNG png(img._width, img._height, img.data);

    return png.writeToFile(path);
}

ImageData::ImageData(const std::string& path)
        : ImageData{load_png(path)}
{
};


ImageData::ImageData(int width, int height)
        : ImageData(width, height, nullptr)
{
    data = new uint8_t[width * height * bpp];
    _external_data = false;
};


ImageData::ImageData(int width, int height, uint8_t* data)
        : _width(width), _height(height), data(data), _external_data(data != nullptr)
{
};


ImageData::ImageData(ImageData&& o) noexcept
        : _width(o._width), _height(o._height), _external_data(o._external_data)
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
    return static_cast<size_t>(_width * _height * bpp);
}

ImageData ImageData::subregion(int x0, int y0, int width, int height)
{
    ImageData temp(width, height);

    for (auto y_ = 0; y_ < height; ++y_)
    {
        for (auto x_ = 0; x_ < width; ++x_)
        {
            auto local_pixel = (y_ + y0)* _width + x_ + x0;
            auto temp_pixel = y_ * width + x_;

            for (auto b = 0u; b < bpp; ++b)
            {
                temp.data[temp_pixel*bpp + b] = data[local_pixel*bpp + b];
            }
        }
    }

    return temp;
}

bool ImageData::save(const string& path)
{
    return save_png(*this, path);
}


