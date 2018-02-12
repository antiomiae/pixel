//
//

#ifndef MAIN_BUFFER_H
#define MAIN_BUFFER_H

#include "common.h"
#include "shader.h"
#include <string>
#include <type_traits>
#include <array>

namespace pixel::graphics
{

class AbstractBuffer
{
public:
    virtual void bind() = 0;
    virtual void unbind() = 0;
};

class Buffer : public AbstractBuffer
{

public:

    GLuint _bufferId{};
    GLenum _usageHint{};

    Buffer();

    explicit Buffer(GLenum usageHint);

    void
    bindToProgramAttribute(
        const Shader& program, const std::string& name, int stride = 0, int offset = 0,
        int divisor = 0
    );

    void loadData(const void* data, const size_t size);

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

public:
    GLuint _bufferId{};

    IndexBuffer();
    GLenum elementType() const;
    void loadData(const T* data, const int size);
    template<size_t length>
    void load(std::array<T, length> data);
    void bind() override;
    void unbind() override;
};


template<typename T>
IndexBuffer<T>::IndexBuffer()
{
    glGenBuffers(1, &_bufferId);
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
}


template<typename T>
void IndexBuffer<T>::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

};


#endif //MAIN_BUFFER_H
