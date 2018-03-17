#include <pixel/graphics/image.h>
#include <pixel/pixel.h>
#include "lua_binding.h"

namespace pixel
{


using namespace std;
using namespace pixel::graphics;

/*
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
*/


void bind_image_data(sol::state& lua, sol::table& binding, const string& type_name)
{
    sol::table t = binding.new_usertype<ImageData>(
        type_name,
        sol::constructors<ImageData(const string&),
            ImageData(unsigned, unsigned),
            ImageData(unsigned, unsigned, uint8_t*),
            ImageData(ImageData&&),
            ImageData(const ImageData&)
        >(),
        "length", &ImageData::length,
        "subregion", &ImageData::subregion,
        "save", &ImageData::save,
        "clear", &ImageData::clear,
        "transpose", &ImageData::transpose,
        /* Wrap these as ints, because Lua doesn't support unsigned types natively */
        "width", sol::property([](ImageData& t) -> int { return (int) t.width; }),
        "height", sol::property([](ImageData& t) -> int { return (int) t.height; }),
        "load_png", &load_png,
        "save_png", &save_png
    );

    t.set("bpp", (int)(ImageData::bpp));
}

};
