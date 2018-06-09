#include "graphics.h"
#include <sstream>
#include <pixel/pixel.h>

using namespace std;

namespace pixel::graphics
{

Vao::Vao()
{
    glGenVertexArrays(1, &vao_id_);
    active_ = false;
}

Vao::Vao(const Vao& other)
    : vao_id_(other.vao_id_), active_(other.active_)
{
}

void Vao::activate()
{
    if (!active_) {
        glBindVertexArray(vao_id_);
        active_ = true;
    }
}

void Vao::deactivate()
{
    if (active_) {
        glBindVertexArray(0);
        active_ = false;
    }
}



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


void Texture::load(unsigned width, unsigned height, const uint8_t* data)
{
    if (texture_type_ == GL_TEXTURE_2D || texture_type_ == GL_TEXTURE_1D_ARRAY) {
        width_ = width;
        height_ = height;

        bind();

        /* TODO: REPLACE THIS */
        glTexImage2D(texture_type_, 0, internal_format_, width, height, 0, format_, data_type_, data);
        log_gl_errors();
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
        pixel_error("Invalid overload of load() called for texture type");
    }
}


void Texture::load(unsigned width, unsigned height, const unsigned depth, const uint8_t* data)
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
        pixel_error("Invalid overload of load() called for texture type");
    }
}


void Texture::unbind() const
{
    glBindTexture(texture_type_, 0);
    log_gl_errors();
}


void Texture::bind() const
{
    glBindTexture(texture_type_, texture_id_);
    log_gl_errors();
}


void debug_print(const Texture& t)
{
    auto level_param = [&](auto e) -> auto {
        int i;
        glGetTexLevelParameteriv(t.texture_type(), 0, e, &i);
        return i;
    };
    auto tex_param = [&](auto e) -> auto {
        int i;
        glGetTexParameteriv(t.texture_type(), e, &i);
        return i;
    };

    t.bind();

    cout << level_param(GL_TEXTURE_WIDTH) << endl;

    t.unbind();
}


void Texture::load_subregion(int x, int y, int width, int height, int layer, const void* data)
{
    if (!allocated_) {
        pixel_error("Texture memory not allocated. Call load() to initialize texture memory.");
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
        pixel_error("Invalid overload of load_subregion() called for texture typed");
    }
}


void Texture::load_subregion(int x, int y, int width, int height, const void* data)
{
    if (!allocated_) {
        pixel_error("Texture memory not allocated. Call load() to initialize texture memory.");
    }

    if (texture_type_ == GL_TEXTURE_2D || texture_type_ == GL_TEXTURE_1D_ARRAY) {
        bind();
        glTexSubImage2D(texture_type_, 0, x, y, width, height, format_, data_type_, data);
        unbind();
    } else {
        pixel_error("Invalid overload of load_subregion() called for texture typed");
    }
}


unsigned format_components(GLenum format)
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


unsigned size_of_gl_type(GLenum type)
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


size_t Texture::storage_size() const
{
    if (data_type_ < GL_BYTE || data_type_ > GL_FLOAT) {
        pixel_error(
            "Only GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT allowed");
    }
    /* Don't try to handle packed formats */
    auto pixels = width_ * height_ * depth_;
    auto components = format_components(format_);
    auto component_size = size_of_gl_type(data_type_);
    return pixels * components * component_size;
}


void Texture::read(void* buf) const
{
    bind();
    glGetTexImage(texture_type_, 0, format_, data_type_, buf);
    log_gl_errors();
    unbind();
}


pixel::graphics::Texture::~Texture()
{
    if (texture_id_ > 0) {
        glDeleteTextures(1, &texture_id_);
        log_gl_errors();
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

unsigned pixel::graphics::Texture::width() const
{ return width_; }

unsigned pixel::graphics::Texture::height() const
{ return height_; }

unsigned pixel::graphics::Texture::depth() const
{ return depth_; }

GLenum pixel::graphics::Texture::texture_type() const
{
    return texture_type_;
}

GLuint pixel::graphics::Texture::texture_id() const
{
    return texture_id_;
}


Camera::Camera(const glm::ivec2& window_size, const glm::vec4& bounds)
    : window_size_{window_size},
      bounds_{bounds}
{
}

void Camera::lock_x(bool lock)
{
    lock_x_ = lock;
}

void Camera::lock_y(bool lock)
{
    lock_y_ = lock;
}

void Camera::translate(float x, float y)
{
    position_ += glm::vec2(x, y) * glm::vec2(lock_x_ ? 0 : 1, lock_y_ ? 0 : 1);
}


void Camera::translate(const glm::vec2& t)
{
    position_ += t * glm::vec2(lock_x_ ? 0 : 1, lock_y_ ? 0 : 1);
}


void Camera::center_at(float x, float y)
{
    position_ = glm::vec2(x, y);
}

void Camera::center_at(const glm::vec2& c)
{
    position_ = c;
}

glm::mat4 Camera::view_matrix() const
{
    return parallax_view_matrix({1.0, 1.0});
}

glm::mat4 Camera::parallax_view_matrix(const glm::vec2& parallax_scale) const
{
    auto center = glm::vec2(window_size_) / 2.0f;
    auto scaled_center = center / scale_;
    auto v = glm::mat4();

    /* move to account for camera position */
    v = glm::translate(v, -glm::vec3(glm::floor(position_ * parallax_scale), 0.0));
    /* translate center of view rect to 0,0 */
    v = glm::translate(glm::mat4(), -glm::vec3(center, 0.0)) * v;
    /* scale */
    auto local_scale = scale_;
    if (parallax_scale == glm::vec2{0.0, 0.0}) {
        local_scale = {1.0, 1.0};
    }

    v = glm::rotate(glm::mat4(), angle_, glm::vec3(0.0f, 0.0f, 1.0f)) * v;

    v = glm::scale(glm::mat4(), glm::vec3(local_scale, 1.0)) * v;

    v = glm::translate(glm::mat4(), glm::vec3(center, 0.0)) * v;

    return v;
}

glm::vec2 Camera::position()
{
    return position_;
}

glm::ivec2 Camera::window_size()
{
    return window_size_;
}

glm::vec2 Camera::scale()
{
    return scale_;
}

void Camera::position_at(float x, float y)
{
    position_at({x, y});
}

void Camera::position_at(const glm::vec2& v)
{
    position_ = v - (glm::vec2(window_size_) / scale_ / 2.0f);
}

void Camera::scale_by(float dsx, float dsy)
{
    scale_ *= glm::vec2(dsx, dsy);
}

void Camera::scale_by(const glm::vec2& s)
{
    scale_ *= s;
}

void Camera::scale(float s)
{
    scale_ = {s, s};
}

void Camera::scale(float x, float y)
{
    scale_ = {x, y};
}

void Camera::scale(const glm::vec2& s)
{
    scale_ = s;
}


void Camera::set_window_size(const glm::ivec2& v)
{
    window_size_ = v;
}

/**
 * The projected region described by this camera's properties
 * \return vec4(x0, y0, x1, y1) a vector of the lower-left and upper-right coordinates of the view rect
 */
glm::vec4 Camera::view_rect()
{
    auto o = glm::vec4(0.0, 0.0, 0.0, 1.0);
    auto m = glm::vec4(glm::vec2(window_size_), 0.0, 1.0);
    auto ivm = glm::inverse(view_matrix());

    o = ivm * o;
    m = ivm * m;

    return glm::vec4(o.x, o.y, m.x, m.y);
}

glm::mat4 Camera::projection_matrix() const
{
    return glm::ortho(0.0f, (float) window_size_.x, 0.0f, (float) window_size_.y);
}

void Camera::set_window_size(int w, int h)
{
    set_window_size({w, h});
}

void Camera::follow(float x, float y)
{
    position_ = glm::vec2(
        lock_x_ ? position_.x : x - (float) window_size_.x / 2.0,
        lock_y_ ? position_.y : y - (float) window_size_.y / 2.0
    );
}

void Camera::follow(const glm::vec2& o)
{
    follow(o.x, o.y);
}

void Camera::set_angle(float a)
{
    angle_ = a;
}



std::string Attribute::debugPrint() const
{
    const pixel::util::SymbolMap& symbols = pixel::util::symbolMap();

    stringstream out;

    out << "Attribute(" << endl;
    out << "  name = " << this->name << endl;
    out << "  location = " << this->location << endl;
    out << "  index = " << this->index << endl;
    out << "  size = " << this->size << endl;
    out << "  type = " << symbols.at(this->type)[0] << endl;
    out << ")" << endl;

    return out.str();
}


};
