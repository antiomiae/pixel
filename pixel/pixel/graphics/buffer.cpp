//
//

#include <iostream>
#include "buffer.h"
#include "../util/util.h"

using pixel::graphics::Attribute;


GLenum basicTypeForAttributeType(GLenum e)
{
    switch (e) {
        case GL_FLOAT:
        case GL_FLOAT_VEC2:
        case GL_FLOAT_VEC3:
        case GL_FLOAT_VEC4:
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
        case GL_INT_VEC4:
            return GL_INT;
        default:
            pixel_error("Unknown enum");
    }
    return 0;
}


int sizeForBasicType(GLenum e)
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
            return sizeof(GLfloat);
        case GL_INT:
        case GL_INT_VEC2:
        case GL_INT_VEC3:
        case GL_INT_VEC4:
            return sizeof(GLint);
        default:
            pixel_error("Unknown enum");
    }
    return 0;
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
        case GL_FLOAT_VEC4:
        case GL_INT_VEC4:
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT3x4:
            return 4;
        default:
            pixel_error("Unknown enum");
    }
    return 0;
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
            pixel_error("Unknown enum");
    }
    return 0;
}


struct Layout
{
    int baseLocation;
    int locationSpan;
    int size;
    GLenum type;

    explicit Layout(const Attribute& attr);
};


Layout::Layout(const Attribute& attr)
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
    : usage_hint_(usageHint)
{
    glGenBuffers(1, &buffer_id_);
}


void pixel::graphics::Buffer::bindToProgramAttribute(
    const Shader& program, const std::string& name, const int stride,
    const int offset, const int divisor
)
{
    bind();

    Attribute attr;

    try {
        attr = program.attribute(name);
    } catch (std::exception& e) {
        std::cout << "bindToProgramAttribute: attribute not found" << std::endl;
        return;
    }

    const Layout layout(attr);

    for (GLuint loc = 0; loc < layout.locationSpan * attr.size; ++loc) {
        if (layout.type == GL_INT) {
            glVertexAttribIPointer(
                loc + layout.baseLocation,
                layout.size,
                layout.type,
                stride,
                reinterpret_cast<void*>(offset + loc * (sizeForBasicType(layout.type) * layout.size))
            );
        } else {
            glVertexAttribPointer(
                loc + layout.baseLocation,
                layout.size,
                layout.type,
                GL_FALSE,
                stride,
                reinterpret_cast<void*>(offset + loc * (sizeForBasicType(layout.type) * layout.size))
            );
        }
        logGlErrors();

        glEnableVertexAttribArray(loc + layout.baseLocation);
        logGlErrors();

        glVertexAttribDivisor(loc + layout.baseLocation, divisor);
        logGlErrors();
    }

    unbind();
}


void pixel::graphics::Buffer::loadData(const void* data, const size_t length)
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, length, data, usage_hint_);
    unbind();
}


void pixel::graphics::Buffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    logGlErrors();
}


void pixel::graphics::Buffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    logGlErrors();
}
