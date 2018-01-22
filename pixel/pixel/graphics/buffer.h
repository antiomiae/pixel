//
//

#ifndef MAIN_BUFFER_H
#define MAIN_BUFFER_H

#include "common.h"
#include "shader.h"
#include <string>

namespace pixel::graphics {

class Buffer {

public:

    GLuint _bufferId;
    GLenum _usageHint;

    Buffer();
    explicit Buffer(const GLenum usageHint);

    void bindToProgramAttribute(const Shader &program, const std::string &name, const int stride = 0, const int offset = 0);
    void loadData(const void *data, const size_t length);
};

};


#endif //MAIN_BUFFER_H
