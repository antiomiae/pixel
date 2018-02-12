#include <pixel/pixel.h>

using namespace pixel::graphics;


Texture::Texture(GLenum texture_type, GLenum format, GLenum internal_format, GLenum data_type)
    :
    _textureType(texture_type),
    _format(format),
    _internalFormat(internal_format),
    _dataType(data_type),
    _allocated(false)
{
    glGenTextures(1, &_textureId);
    bind();
    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind();
}


void Texture::load(const int width, const int height, const uint8_t* data)
{
    if (_textureType == GL_TEXTURE_2D || _textureType == GL_TEXTURE_1D_ARRAY) {
        _width = width;
        _height = height;

        bind();

        /* TODO: REPLACE THIS */
        glTexImage2D(_textureType, 0, _internalFormat, width, height, 0, _format, _dataType, data);
        logGlErrors();
        /* WITH THIS:
        if (data != nullptr) {
            glTexImage2D(_textureType, 0, _format, width, height, 0, _format, _dataType, data);
        } else {
            // Determine the sized format from our format and data type
            auto f = getSizedFormat(_format, _dataType);
            // Or require this class uses only sized formats and discard our datatype field
            glTexStorage2D(_textureType, 1, f, width, height);
        }
        */

        unbind();

        _allocated = true;
    } else {
        throw "Invalid overload of load() called for texture type";
    }
}


void Texture::unbind() const
{
    glBindTexture(_textureType, 0);
    logGlErrors();
}


void Texture::bind() const
{
    glBindTexture(_textureType, _textureId);
    logGlErrors();
}


void Texture::load(const int width, const int height, const int depth, const uint8_t* data)
{
    if (_textureType == GL_TEXTURE_3D || _textureType == GL_TEXTURE_2D_ARRAY) {
        bind();
        glTexImage3D(_textureType, 0, _format, width, height, depth, 0, _format, _dataType, data);
        unbind();

        _width = width;
        _height = height;
        _depth = depth;
        _allocated = true;
    } else {
        throw "Invalid overload of load() called for texture type";
    }
}


void debugPrint(const Texture& t)
{
    auto level_param = [&](auto e) -> auto {
        int i;
        glGetTexLevelParameteriv(t._textureType, 0, e, &i);
        return i;
    };
    auto tex_param = [&](auto e) -> auto {
        int i;
        glGetTexParameteriv(t._textureType, e, &i);
        return i;
    };

    t.bind();

    cout << level_param(GL_TEXTURE_WIDTH) << endl;

    t.unbind();
}


void Texture::loadSubregion(int x, int y, int width, int height, int layer, const void* data)
{
    if (!_allocated) {
        throw "Texture memory not allocated. Call load() to initialize texture memory.";
    }

    if (x + width > _width) {
        error("subregion exceeds width of texture");
    }
    if (y + height > _height) {
        error("subregion exceeds height of texture");
    }
    if (layer >= _depth) {
        error("layer exceeds depth of texture");
    }

    if (_textureType == GL_TEXTURE_3D || _textureType == GL_TEXTURE_2D_ARRAY) {
        bind();
        glTexSubImage3D(_textureType, 0, x, y, layer, width, height, 1, _format, _dataType, data);
        unbind();
    } else {
        throw "Invalid overload of loadSubregion() called for texture typed";
    }
}


void Texture::loadSubregion(int x, int y, int width, int height, const void* data)
{
    if (!_allocated) {
        throw "Texture memory not allocated. Call load() to initialize texture memory.";
    }

    if (_textureType == GL_TEXTURE_2D || _textureType == GL_TEXTURE_1D_ARRAY) {
        bind();
        glTexSubImage2D(_textureType, 0, x, y, width, height, _format, _dataType, data);
        unbind();
    } else {
        throw "Invalid overload of loadSubregion() called for texture typed";
    }
}


unsigned formatComponents(GLenum format)
{
    switch (format) {
        case GL_RED:
        case GL_RED_INTEGER:
        case GL_STENCIL_INDEX:
        case GL_DEPTH_COMPONENT:
            return 1;
        case GL_RG:
        case GL_RG_INTEGER:
        case GL_DEPTH_STENCIL:
            return 2;
        case GL_RGB:
        case GL_BGR:
        case GL_RGB_INTEGER:
        case GL_BGR_INTEGER:
            return 3;
        case GL_RGBA:
        case GL_BGRA:
        case GL_RGBA_INTEGER:
        case GL_BGRA_INTEGER:
            return 4;
        default:
            error("Unknown format");
            return 0;
    }
}


unsigned sizeofGLType(GLenum type)
{
    switch (type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            return sizeof(GLshort);
        case GL_INT:
        case GL_UNSIGNED_INT:
            return sizeof(GLint);
        case GL_FLOAT:
            return sizeof(GLfloat);
        default:
            error("Invalid GL type");
            return 0;
    }
}


size_t Texture::storageSize() const
{
    if (_dataType < GL_BYTE || _dataType > GL_FLOAT) {
        error("Only GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT allowed");
    }
    /* Don't try to handle packed formats */
    auto pixels = _width * _height * _depth;
    auto components = formatComponents(_format);
    auto component_size = sizeofGLType(_dataType);
    return pixels * components * component_size;
}


void Texture::read(void* buf)
{
    bind();
    glGetTexImage(_textureType, 0, _format, _dataType, buf);
    logGlErrors();
    unbind();
}


pixel::graphics::Texture::~Texture()
{
    if (_textureId > 0) {
        glDeleteTextures(1, &_textureId);
        logGlErrors();
        _textureId = 0;
    }
}


pixel::graphics::Texture::Texture(Texture&& rhs) noexcept
    : _textureId(rhs._textureId),
      _textureType(rhs._textureType),
      _format(rhs._format),
      _internalFormat(rhs._internalFormat),
      _dataType(rhs._dataType),
      _width(rhs._width),
      _height(rhs._height),
      _depth(rhs._depth),
      _allocated(rhs._allocated)
{
    rhs._textureId = 0;
}
