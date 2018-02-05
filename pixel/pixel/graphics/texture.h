#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "common.h"
#include <iostream>

using namespace std;

namespace pixel::graphics
{

class Texture
{
public:
    GLuint _textureId;
    GLenum _textureType;
    GLenum _format;
    GLenum _dataType;

    int _width;
    int _height;
    int _depth;

    bool _allocated;

    explicit Texture(GLenum textureType = GL_TEXTURE_2D, GLenum format = GL_RGBA, GLenum dataType = GL_UNSIGNED_BYTE);

    unsigned width() const { return _width; };
    unsigned height() const { return _height; };
    unsigned depth() const { return _depth; };

    void load(int width, int height, const uint8_t* data = nullptr);
    void load(int width, int height, int depth, const uint8_t* data = nullptr);
    void loadSubregion(int x, int y, int width, int height, const uint8_t* data);
    void loadSubregion(int x, int y, int width, int height, int layer, const uint8_t* data);
    void bind() const;
    void unbind() const;

    size_t storageSize() const;

    void read(void* buf);
};

};


#endif //GRAPHICS_TEXTURE_H
