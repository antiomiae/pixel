#ifndef GL_COMMON_H
#define GL_COMMON_H

#define GLFW_INCLUDE_NONE

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <string>
#include <vector>
#include "image.h"
#include "common.h"
#include <pixel/math.h>

using namespace std;

namespace pixel::graphics
{

class Vao
{
public:
    GLuint vao_id_;
    bool active_;
    Vao();
    Vao(const Vao& other);

    void activate();
    void deactivate();
};


struct Attribute
{
    unsigned int index;
    int size;
    int location;
    unsigned int type;
    std::string name;

    std::string debugPrint() const;
};

using AttributeMap = std::unordered_map<std::string, Attribute>;

class Shader
{
public:
    GLuint program_id_;
    AttributeMap attribute_map_;
    AttributeMap uniform_map_;

    std::string debug_name_;

    Shader() = default;
    Shader(const char* vs, const char* fs, const vector<string>& transform_feedback_varyings = {});

    void activate();
    void deactivate();

    Attribute attribute(const std::string& name) const;
    Attribute uniform(const std::string& name) const;

    void setUniform(const std::string& name, int);
    void setUniform(const std::string& name, int, int);
    void setUniform(const std::string& name, int, int, int);

    void setUniform(const std::string& name, float);
    void setUniform(const std::string& name, float, float);
    void setUniform(const std::string& name, float, float, float);
    void setUniform(const std::string& name, float, float, float, float);

    void setUniform(const std::string& name, const glm::vec2& value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);

    void setUniform(const std::string& name, const glm::ivec2& value);

    void setUniform(const std::string& name, const glm::mat2& value);
    void setUniform(const std::string& name, const glm::mat3& value);
    void setUniform(const std::string& name, const glm::mat4& value);

    void setUniformArray(const std::string& name, int count, const float* arr);
    void setUniformArray(const std::string& name, int count, const int* arr);

    void setUniformArray(const std::string& name, int count, const glm::vec2* arr);
    void setUniformArray(const std::string& name, int count, const glm::vec4* arr);
    void setUniformArray(const std::string& name, int count, const glm::vec3* arr);

    void setUniformArray(const std::string& name, int count, const glm::mat2* arr);
    void setUniformArray(const std::string& name, int count, const glm::mat3* arr);
    void setUniformArray(const std::string& name, int count, const glm::mat4* arr);

    std::string debugPrint() const;

};

class AbstractBuffer
{
public:
    virtual void bind() = 0;
    virtual void unbind() = 0;
};

class Buffer : public AbstractBuffer
{

public:

    GLuint buffer_id_{};
    GLenum usage_hint_{};

    Buffer();

    explicit Buffer(GLenum usageHint);

    void
    bindToProgramAttribute(
        const Shader& program, const std::string& name, int stride = 0, int offset = 0,
        int divisor = 0
    );

    void loadData(const void* data, const size_t size);

    template<class Elem>
    auto load_data(const vector<Elem>& v)
    {
        loadData(v.data(), v.size() * sizeof(v));
    }

    void bind() override;
    void unbind() override;
};

template<typename T>
class IndexBuffer : public AbstractBuffer
{
    static_assert(
        std::is_same<GLubyte, T>::value ||
        std::is_same<GLushort, T>::value ||
        std::is_same<GLuint, T>::value,
        "Only unsigned char, short, and int types allowed"
    );

private:
    GLuint buffer_id_{};

public:
    IndexBuffer();
    GLenum elementType() const;
    void loadData(const T* data, const int size);
    template<size_t length>
    void load(std::array<T, length> data);
    void bind() override;
    void unbind() override;
    GLuint buffer_id() const;
};


template<typename T>
IndexBuffer<T>::IndexBuffer()
{
    glGenBuffers(1, &buffer_id_);
}


template<typename T>
template<size_t length>
void IndexBuffer<T>::load(std::array<T, length> data)
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(T), data.data(), GL_STATIC_DRAW);
    unbind();
}


template<typename T>
void IndexBuffer<T>::loadData(const T* data, const int size)
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    unbind();
}


template<typename T>
GLenum IndexBuffer<T>::elementType() const
{
    if (std::is_same<GLubyte, T>::value) {
        return GL_UNSIGNED_BYTE;
    } else if (std::is_same<GLushort, T>::value) {
        return GL_UNSIGNED_SHORT;
    } else {
        return GL_UNSIGNED_INT;
    }
}


template<typename T>
void IndexBuffer<T>::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
}


template<typename T>
void IndexBuffer<T>::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


template<typename T>
GLuint IndexBuffer<T>::buffer_id() const
{
    return buffer_id_;
}


class OffscreenRenderTarget
{
public:
    OffscreenRenderTarget();

    void activate();
    void deactivate();
    void draw(const glm::ivec4& draw_region);
    glm::ivec2 window_size() const;
    void set_window_size(const glm::ivec2&);
    void set_window_size(int, int);

private:
    unsigned fbo_;
    unsigned rbo_color_;
    unsigned rbo_depth_stencil_;
    glm::ivec2 size_;
    glm::ivec4 old_viewport_;

    void resize_buffers(unsigned int width, unsigned int height);
    void attach_buffers();
};

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
    Texture(Texture&& rhs) noexcept;

    Texture& operator=(Texture&) = delete;
    Texture& operator=(Texture&&);

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


struct TextureRegion
{
    union
    {
        struct
        {
            int32_t x;
            int32_t y;
            int32_t w;
            int32_t h;
        };

        int32_t rect[4];

        glm::ivec4 vec{};
    };

    int32_t layer;

    /**
     * Whether pixel data should be transposed when used
     */
    int flipped;

    TextureRegion() = default;

    TextureRegion(int x, int y, int w, int h, int layer, bool flipped)
        : x{x},
          y{y},
          w{w},
          h{h},
          layer{layer},
          flipped(flipped)
    {
    }
};


class TextureAtlas
{
public:

    using RegionMap = unordered_map<uint32_t, TextureRegion>;
    using RegionMapItem = pair<uint32_t, TextureRegion>;

    const unsigned kDefaultWidth = 256;
    const unsigned kDefaultHeight = 256;
    const unsigned kDefaultLayers = 10;

    TextureAtlas() = default;

    explicit TextureAtlas(glm::uvec3 size);

    TextureAtlas(const TextureAtlas&) = delete;

    void start_batch();
    void stop_batch();

    uint32_t add_image(const string& path);
    uint32_t add_image(const string& path, const string& name);
    uint32_t add_image(const ImageData& img, const string& name);

    vector<ImageData>& layers();

    TextureRegion lookup(const std::string& name) const;
    TextureRegion lookup(uint32_t region_id) const;

    const RegionMap& region_map() const { return tex_regions_; }
    const unordered_map<string, uint32_t>& name_registry() const { return name_registry_; };

    glm::uvec3 atlas_size() const
    {
        return atlas_size_;
    }

    string debug_print() const;

    Texture as_texture() const;


private:
    struct ImageSize
    {
        unsigned w, h;
        uint32_t region_id;

        ImageSize(unsigned w, unsigned h, uint32_t region_id)
            : w{w},
              h{h},
              region_id{region_id}
        {
        }
    };

    uint32_t top_id_{0};
    unordered_map<string, uint32_t> name_registry_{};
    RegionMap tex_regions_{};
    vector<ImageData> layers_{};
    vector<ImageSize> blocks_{};
    unordered_map<uint32_t, ImageData> image_buffers_{};

    glm::uvec3 atlas_size_ = {
        kDefaultWidth,
        kDefaultHeight,
        kDefaultLayers
    };

    void process_batch();
};

class Camera
{
public:
    Camera(const glm::ivec2& window_size, const glm::vec4& bounds);
    Camera() = default;

    void lock_x(bool lock = true);
    void lock_y(bool lock = true);

    void translate(float x, float y);
    void translate(const glm::vec2&);

    /**
     * Place camera so that (x, y) is at center of frame
     * \param x
     * \param y
     */
    void center_at(float x, float y);
    void center_at(const glm::vec2&);

    /**
     * Place camera so that (x, y) is at center of frame, respecting camera axis locks.
     * \param x
     * \param y
     */
    void follow(float x, float y);
    void follow(const glm::vec2&);

    void position_at(float x, float y);
    void position_at(const glm::vec2&);

    void scale_by(float dsx, float dsy);
    void scale_by(const glm::vec2&);

    void scale(float s);
    void scale(float x, float y);
    void scale(const glm::vec2&);

    void set_window_size(const glm::ivec2& v);
    void set_window_size(int w, int h);

    void set_angle(float);

    glm::mat4 view_matrix() const;
    glm::mat4 parallax_view_matrix(const glm::vec2&) const;
    glm::mat4 projection_matrix() const;

    glm::vec2 position();
    glm::vec2 scale();
    glm::ivec2 window_size();
    glm::vec4 view_rect();

private:
    bool lock_x_{false};
    bool lock_y_{false};

    glm::ivec2 window_size_{320, 240};
    glm::vec4 bounds_{0, 0, 512, 512};
    glm::vec2 position_{0.0, 0.0};
    glm::vec2 scale_{1.0};
    float angle_{0.0};
};

};


#endif //GL_COMMON_H
