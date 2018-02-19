//
//

#ifndef GRAPHICS_VAO_H
#define GRAPHICS_VAO_H

#include "common.h"

namespace pixel::graphics {

class Vao {
public:
    GLuint vao_id_;
    bool active_;
    Vao();
    Vao(const Vao& other);

    void activate();
    void deactivate();
};

};

#endif //GRAPHICS_VAO_H
