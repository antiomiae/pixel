#include "image.h"

#include <tinypng/png.h>
#include "../util/util.h"
#include "../error.h"

using namespace pixel::graphics;
using namespace pixel::util;


ImageData pixel::graphics::load_png(const string &path)
{
    if (!file_exists(path)) {
        pixel_error("file does not exist at path: " + path);
    }

    tinypng::PNG png(path);

    ImageData out(png.getWidth(), png.getHeight());

    memcpy(out.data, png.buffer(), out.length());

    return out;
};


bool pixel::graphics::save_png(const ImageData &img, const std::string &path)
{
    tinypng::PNG png(img.width, img.height, img.data);

    return png.writeToFile(path);
}


ImageData::ImageData(const std::string &path)
        : ImageData{load_png(path)}
{
};


ImageData::ImageData(unsigned int _width, unsigned int _height)
        : ImageData(_width, _height, nullptr)
{
    data = new uint8_t[width * height * bpp];
    external_data = false;
};


ImageData::ImageData(unsigned int _width, unsigned int _height, uint8_t *_data)
        : width(_width), height(_height), data(_data), external_data(_data != nullptr)
{
};


ImageData::ImageData(ImageData &&o) noexcept
        : width(o.width), height(o.height), external_data(o.external_data)
{
    data = o.data;
    o.data = nullptr;
};

ImageData::ImageData(const ImageData &o)
        : ImageData(o.width, o.height)
{
    if (o.data != nullptr) {
        memcpy(data, o.data, width * height * bpp);
    }
}


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


ImageData ImageData::subregion(unsigned int x0, unsigned int y0, unsigned int _width, unsigned int _height) const
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


bool ImageData::save(const string &path) const
{
    return save_png(*this, path);
}

bool
ImageData::load_subregion(const ImageData &other,
                          unsigned src_x,
                          unsigned src_y,
                          unsigned src_width,
                          unsigned src_height,
                          unsigned dest_x,
                          unsigned dest_y)
{
    argument_error_if(dest_x + src_width > width || dest_y + src_height > height,
                      "Destination rect goes outside image bounds");
    argument_error_if(src_x + src_width > other.width || src_y + src_height > other.height,
                      "Source rect goes outside source image bounds");

    for (auto y = 0u; y < src_height; ++y) {
        for (auto x = 0u; x < src_width; ++x) {
            for (auto b = 0u; b < bpp; ++b) {
                auto src_pixel = other.data[(x + src_x + (y + src_y) * other.width) * bpp + b];

                data[(dest_x + x + (dest_y + y) * width) * bpp + b] = src_pixel;
            }
        }
    }

    return true;
}

void ImageData::clear()
{
    if (data != nullptr && !external_data) {
        memset(data, 0, length());
    }
}
