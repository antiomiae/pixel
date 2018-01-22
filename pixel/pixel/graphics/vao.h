//
//

#ifndef GRAPHICS_VAO_H
#define GRAPHICS_VAO_H

#include "common.h"

namespace pixel::graphics {

class Vao {
public:
    GLuint _vaoId;
    bool _active;
    Vao();
    Vao(const Vao& other);

    void activate();
    void deactivate();
};

};

#endif //GRAPHICS_VAO_H
