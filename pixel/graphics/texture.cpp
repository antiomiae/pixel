#include "texture.h"

using namespace pixel::graphics;

Texture::Texture(GLenum texture_type, GLenum format, GLenum data_type) :
        _textureType(texture_type),
        _format(format),
        _dataType(data_type)
{
    glGenTextures(1, &_textureId);
    glBindTexture(_textureType, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glBindTexture(_textureType, 0);
}


void Texture::load(const int width, const int height, const uint8_t *data)
{
    if (_textureType == GL_TEXTURE_2D || _textureType == GL_TEXTURE_1D_ARRAY) {
        glBindTexture(_textureType, _textureId);
        glTexImage2D(_textureType, 0, _format, width, height, 0, _format, _dataType, data);
        glBindTexture(_textureType, 0);

        _width = width;
        _height = height;
    } else {
        throw "Invalid overload of load() called for texture type";
    }
}


void Texture::load(const int width, const int height, const int depth, const uint8_t *data)
{
    if (_textureType == GL_TEXTURE_3D || _textureType == GL_TEXTURE_2D_ARRAY) {
        glBindTexture(_textureType, _textureId);
        glTexImage3D(_textureType, 0, _format, width, height, depth, 0, _format, _dataType, data);
        glBindTexture(_textureType, 0);

        _width = width;
        _height = height;
        _depth = depth;
    } else {
        throw "Invalid overload of load() called for texture type";
    }
}
