#ifndef GRAPHICS_IMAGE_H
#define GRAPHICS_IMAGE_H

#include <string>
#include <cstdint>

namespace pixel::graphics
{

struct ImageData
{
    unsigned width, height;
    uint8_t* data;
    bool external_data;

    explicit ImageData(const std::string& path);

    ImageData(unsigned int width, unsigned int height);
    ImageData(unsigned int _width, unsigned int _height, uint8_t* _data);
    ImageData(ImageData&& other) noexcept;
    ImageData(const ImageData&);
    ImageData& operator=(const ImageData&);
    ~ImageData();

    size_t length();

    ImageData subregion(unsigned int x0, unsigned int y0, unsigned int _width, unsigned int _height) const;

    bool load_subregion(const ImageData&, unsigned src_x, unsigned src_y, unsigned src_width, unsigned src_height, unsigned dest_x, unsigned dest_y);

    bool save(const std::string& path) const;
    void clear();

    ImageData transpose() const;

    static const int bpp = 4;
};

ImageData load_png(const std::string& path);

bool save_png(const ImageData& img, const std::string& path);

};

#endif //GRAPHICS_IMAGE_H
