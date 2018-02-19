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
    GLuint texture_id_{};
    GLenum texture_type_{};
    GLenum format_{};
    GLenum internal_format_;
    GLenum data_type_{};

    int width_;
    int height_;
    int depth_;

    bool allocated_;

    explicit Texture(
        GLenum textureType = GL_TEXTURE_2D, GLenum format = GL_RGBA, GLenum internal_format = GL_RGBA,
        GLenum dataType = GL_UNSIGNED_BYTE
    );
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(Texture&) = delete;

    Texture(Texture&& rhs) noexcept;


    unsigned width() const
    { return width_; };


    unsigned height() const
    { return height_; };


    unsigned depth() const
    { return depth_; };

    void load(int width, int height, const uint8_t* data = nullptr);
    void load(int width, int height, int depth, const uint8_t* data = nullptr);
    void loadSubregion(int x, int y, int width, int height, const void* data);
    void loadSubregion(int x, int y, int width, int height, int layer, const void* data);
    void bind() const;
    void unbind() const;
    void activate(unsigned unit) const;

    size_t storageSize() const;

    void read(void* buf);
};

};


#endif //GRAPHICS_TEXTURE_H
