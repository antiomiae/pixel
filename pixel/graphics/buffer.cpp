//
//

#include "buffer.h"


GLenum basicTypeForAttributeType(GLenum e) {
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

int sizeForAttributeType(GLenum e) {
    switch (e) {
        case GL_FLOAT:
        case GL_INT:
            return 1;
        case GL_FLOAT_VEC2:
        case GL_INT_VEC2:
            return 2;
        case GL_FLOAT_VEC3:
        case GL_INT_VEC3:
            return 3;
        case GL_FLOAT_MAT2:
            return 4;
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT3x2:
            return 6;
        case GL_FLOAT_MAT3:
            return 9;
        case GL_FLOAT_MAT3x4:
        case GL_FLOAT_MAT4x3:
            return 12;
        case GL_FLOAT_MAT4:
            return 16;
        default:
            return 1;
    }
}

pixel::graphics::Buffer::Buffer()
        : Buffer(GL_STATIC_DRAW) {
}


pixel::graphics::Buffer::Buffer(const GLenum usageHint)
        : _usageHint(usageHint) {
    glGenBuffers(1, &_bufferId);
}

void pixel::graphics::Buffer::bindToProgramAttribute(const Shader &program, const std::string &name, const int stride,
                                                     const int offset) {
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);

    auto attr = program.attribute(name);

    auto size = attr.size * sizeForAttributeType(attr.type);
    auto type = basicTypeForAttributeType(attr.type);

    glVertexAttribPointer(
            attr.index,
            size,
            type,
            GL_FALSE,
            stride,
            (void *) offset
    );

    glEnableVertexAttribArray(attr.index);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void pixel::graphics::Buffer::loadData(void *data, const size_t length) {
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);

    glBufferData(GL_ARRAY_BUFFER, length, data, _usageHint);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
