#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "common.h"
#include <iostream>

namespace pixel::graphics
{

using namespace std;

class Texture
{
public:
    explicit Texture(
        GLenum textureType = GL_TEXTURE_2D,
        GLenum format = GL_RGBA,
        GLenum internal_format = GL_RGBA,
        GLenum dataType = GL_UNSIGNED_BYTE
    );

    ~Texture();

    Texture(const Texture&) = delete;

    Texture& operator=(Texture&) = delete;

    Texture(Texture&& rhs) noexcept;

    unsigned width() const;
    unsigned height() const;
    unsigned depth() const;
    GLenum texture_type() const;
    GLuint texture_id() const;

    void load(unsigned width, unsigned height, const uint8_t* data = nullptr);
    void load(unsigned width, unsigned height, unsigned depth, const uint8_t* data = nullptr);

    void load_subregion(int x, int y, int width, int height, const void* data);
    void load_subregion(int x, int y, int width, int height, int layer, const void* data);

    void bind() const;
    void unbind() const;

    void activate(unsigned unit) const;

    size_t storage_size() const;

    void read(void* buf) const;

private:
    GLuint texture_id_{};
    GLenum texture_type_{};
    GLenum format_{};
    GLenum internal_format_;
    GLenum data_type_{};

    unsigned width_{};
    unsigned height_{};
    unsigned depth_{};

    bool allocated_{};
};

};


#endif //GRAPHICS_TEXTURE_H
