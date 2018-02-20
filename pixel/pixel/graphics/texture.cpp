#include <pixel/pixel.h>

using namespace pixel::graphics;


Texture::Texture(GLenum texture_type, GLenum format, GLenum internal_format, GLenum data_type)
    :
    texture_type_(texture_type),
    format_(format),
    internal_format_(internal_format),
    data_type_(data_type),
    allocated_(false)
{
    glGenTextures(1, &texture_id_);
    bind();
    glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind();
}


void Texture::load(const int width, const int height, const uint8_t* data)
{
    if (texture_type_ == GL_TEXTURE_2D || texture_type_ == GL_TEXTURE_1D_ARRAY) {
        width_ = width;
        height_ = height;

        bind();

        /* TODO: REPLACE THIS */
        glTexImage2D(texture_type_, 0, internal_format_, width, height, 0, format_, data_type_, data);
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

        allocated_ = true;
    } else {
        throw "Invalid overload of load() called for texture type";
    }
}


void Texture::unbind() const
{
    glBindTexture(texture_type_, 0);
    logGlErrors();
}


void Texture::bind() const
{
    glBindTexture(texture_type_, texture_id_);
    logGlErrors();
}


void Texture::load(const int width, const int height, const int depth, const uint8_t* data)
{
    if (texture_type_ == GL_TEXTURE_3D || texture_type_ == GL_TEXTURE_2D_ARRAY) {
        bind();
        glTexImage3D(texture_type_, 0, format_, width, height, depth, 0, format_, data_type_, data);
        unbind();

        width_ = width;
        height_ = height;
        depth_ = depth;
        allocated_ = true;
    } else {
        throw "Invalid overload of load() called for texture type";
    }
}


void debugPrint(const Texture& t)
{
    auto level_param = [&](auto e) -> auto {
        int i;
        glGetTexLevelParameteriv(t.texture_type_, 0, e, &i);
        return i;
    };
    auto tex_param = [&](auto e) -> auto {
        int i;
        glGetTexParameteriv(t.texture_type_, e, &i);
        return i;
    };

    t.bind();

    cout << level_param(GL_TEXTURE_WIDTH) << endl;

    t.unbind();
}


void Texture::loadSubregion(int x, int y, int width, int height, int layer, const void* data)
{
    if (!allocated_) {
        throw "Texture memory not allocated. Call load() to initialize texture memory.";
    }

    if (x + width > width_) {
        pixel_error("subregion exceeds width of texture");
    }
    if (y + height > height_) {
        pixel_error("subregion exceeds height of texture");
    }
    if (layer >= depth_) {
        pixel_error("layer exceeds depth of texture");
    }

    if (texture_type_ == GL_TEXTURE_3D || texture_type_ == GL_TEXTURE_2D_ARRAY) {
        bind();
        glTexSubImage3D(texture_type_, 0, x, y, layer, width, height, 1, format_, data_type_, data);
        unbind();
    } else {
        throw "Invalid overload of loadSubregion() called for texture typed";
    }
}


void Texture::loadSubregion(int x, int y, int width, int height, const void* data)
{
    if (!allocated_) {
        throw "Texture memory not allocated. Call load() to initialize texture memory.";
    }

    if (texture_type_ == GL_TEXTURE_2D || texture_type_ == GL_TEXTURE_1D_ARRAY) {
        bind();
        glTexSubImage2D(texture_type_, 0, x, y, width, height, format_, data_type_, data);
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
            pixel_error("Unknown format");
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
            pixel_error("Invalid GL type");
            return 0;
    }
}


size_t Texture::storageSize() const
{
    if (data_type_ < GL_BYTE || data_type_ > GL_FLOAT) {
        pixel_error("Only GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT allowed");
    }
    /* Don't try to handle packed formats */
    auto pixels = width_ * height_ * depth_;
    auto components = formatComponents(format_);
    auto component_size = sizeofGLType(data_type_);
    return pixels * components * component_size;
}


void Texture::read(void* buf)
{
    bind();
    glGetTexImage(texture_type_, 0, format_, data_type_, buf);
    logGlErrors();
    unbind();
}


pixel::graphics::Texture::~Texture()
{
    if (texture_id_ > 0) {
        glDeleteTextures(1, &texture_id_);
        logGlErrors();
        texture_id_ = 0;
    }
}


pixel::graphics::Texture::Texture(Texture&& rhs) noexcept
    : texture_id_(rhs.texture_id_),
      texture_type_(rhs.texture_type_),
      format_(rhs.format_),
      internal_format_(rhs.internal_format_),
      data_type_(rhs.data_type_),
      width_(rhs.width_),
      height_(rhs.height_),
      depth_(rhs.depth_),
      allocated_(rhs.allocated_)
{
    rhs.texture_id_ = 0;
}


void pixel::graphics::Texture::activate(unsigned unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    bind();
}
