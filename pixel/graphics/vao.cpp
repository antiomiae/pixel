//
//

#include "vao.h"

using namespace pixel::graphics;

Vao::Vao() {
    glGenVertexArrays(1, &_vaoId);
    _active = false;
}

Vao::Vao(const pixel::graphics::Vao &other)
        : _vaoId(other._vaoId), _active(other._active)
{
}

void Vao::activate() {
    if (!_active) {
        glBindVertexArray(_vaoId);
        _active = true;
    }
}

void Vao::deactivate() {
    if (_active) {
        glBindVertexArray(0);
        _active = false;
    }
}
