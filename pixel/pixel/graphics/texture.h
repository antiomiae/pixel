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
    GLuint _textureId{};
    GLenum _textureType{};
    GLenum _format{};
    GLenum _internalFormat;
    GLenum _dataType{};

    int _width;
    int _height;
    int _depth;

    bool _allocated;

    explicit Texture(
        GLenum textureType = GL_TEXTURE_2D, GLenum format = GL_RGBA, GLenum internal_format = GL_RGBA,
        GLenum dataType = GL_UNSIGNED_BYTE
    );
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(Texture&) = delete;

    Texture(Texture&& rhs) noexcept;


    unsigned width() const
    { return _width; };


    unsigned height() const
    { return _height; };


    unsigned depth() const
    { return _depth; };

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
