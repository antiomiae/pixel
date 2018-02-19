//
//

#include "vao.h"

using namespace pixel::graphics;

Vao::Vao() {
    glGenVertexArrays(1, &vao_id_);
    active_ = false;
}

Vao::Vao(const pixel::graphics::Vao &other)
        : vao_id_(other.vao_id_), active_(other.active_)
{
}

void Vao::activate() {
    if (!active_) {
        glBindVertexArray(vao_id_);
        active_ = true;
    }
}

void Vao::deactivate() {
    if (active_) {
        glBindVertexArray(0);
        active_ = false;
    }
}
