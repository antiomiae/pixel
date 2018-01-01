//
//

#include "buffer.h"
#include "../util/util.h"

using pixel::graphics::Attribute;

GLenum basicTypeForAttributeType(GLenum e)
{
    switch (e) {
        case GL_FLOAT:
        case GL_FLOAT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT2x4:
        case GL_FLOAT_MAT4x2:
        case GL_FLOAT_MAT3x4:
        case GL_FLOAT_MAT4x3:
            return GL_FLOAT;
        case GL_INT:
        case GL_INT_VEC2:
        case GL_INT_VEC3:
            return GL_INT;
        default:
            return GL_DOUBLE;
    }
}

int sizeForBasicType(GLenum e) {
    switch (e) {
        case GL_FLOAT:
        case GL_FLOAT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT2x4:
        case GL_FLOAT_MAT4x2:
        case GL_FLOAT_MAT3x4:
        case GL_FLOAT_MAT4x3:
            return sizeof(GLfloat);
        case GL_INT:
        case GL_INT_VEC2:
        case GL_INT_VEC3:
            return sizeof(GLint);
        default:
            return sizeof(GLdouble);
    }
}

int componentsForAttributeType(GLenum e)
{
    switch (e) {
        case GL_FLOAT:
        case GL_INT:
            return 1;
        case GL_FLOAT_VEC2:
        case GL_INT_VEC2:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT2:
            return 2;
        case GL_FLOAT_VEC3:
        case GL_INT_VEC3:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT4x3:
            return 3;
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT3x4:
            return 4;
        default:
            return 1;
    }
}

int locationSpanForAttributeType(GLenum e)
{
    switch (e) {
        case GL_FLOAT:
        case GL_INT:
        case GL_FLOAT_VEC2:
        case GL_INT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_INT_VEC3:
        case GL_FLOAT_VEC4:
        case GL_INT_VEC4:
            return 1;
        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT2x3:
            return 2;
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT3x4:
            return 3;
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT4x3:
            return 4;
        default:
            return 1;
    }
}

struct Layout
{
    int baseLocation;
    int locationSpan;
    int size;
    GLenum type;

    explicit Layout(const Attribute &attr);
};

Layout::Layout(const Attribute &attr)
        : baseLocation(attr.location),
          locationSpan(locationSpanForAttributeType(attr.type)),
          size(componentsForAttributeType(attr.type)),
          type(basicTypeForAttributeType(attr.type))
{
}

pixel::graphics::Buffer::Buffer()
        : Buffer(GL_STATIC_DRAW)
{
}


pixel::graphics::Buffer::Buffer(const GLenum usageHint)
        : _usageHint(usageHint)
{
    glGenBuffers(1, &_bufferId);
}

void pixel::graphics::Buffer::bindToProgramAttribute(const Shader &program, const std::string &name, const int stride,
                                                     const int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    logGlErrors();

    auto attr = program.attribute(name);

    const Layout layout(attr);

    for (GLuint loc = 0; loc < layout.locationSpan; ++loc) {
        glVertexAttribPointer(
                loc + layout.baseLocation,
                layout.size,
                layout.type,
                GL_FALSE,
                stride,
                reinterpret_cast<void *>(offset + loc * (sizeForBasicType(layout.type) * layout.size))
        );
        logGlErrors();

        glEnableVertexAttribArray(loc + layout.baseLocation);
        logGlErrors();
    }

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    logGlErrors();
}

void pixel::graphics::Buffer::loadData(const void *data, const size_t length)
{
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);

    glBufferData(GL_ARRAY_BUFFER, length, data, _usageHint);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
