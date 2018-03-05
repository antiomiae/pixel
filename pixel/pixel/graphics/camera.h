

#ifndef PIXEL_CAMERA_H
#define PIXEL_CAMERA_H

#include <pixel/math/math.h>

namespace pixel::graphics
{

class Camera
{
public:
    Camera(glm::ivec2 window_size, glm::vec4 bounds);

    void lock_x(bool lock = true);
    void lock_y(bool lock = true);

    void translate(float x, float y);
    void translate(glm::vec2&);

    void center_at(float x, float y);
    void center_at(glm::vec2&);

    glm::mat4 view_matrix();
private:
    bool lock_x_{false};
    bool lock_y_{false};

    glm::ivec2 window_size_;
    glm::vec4 bounds_{0, 0, 512, 512};
    glm::vec2 position_{0.0, 0.0};
    glm::vec2 scale_{1.0};
};

};

#endif //PIXEL_CAMERA_H
