

#include "camera.h"

pixel::graphics::Camera::Camera(glm::ivec2 window_size, glm::vec4 bounds)
    : window_size_{window_size},
      bounds_{bounds}
{

}

void pixel::graphics::Camera::lock_x(bool lock)
{
    lock_y_ = lock;
}

void pixel::graphics::Camera::lock_y(bool lock)
{
    lock_y_ = lock;
}

void pixel::graphics::Camera::translate(float x, float y)
{
    position_ += glm::vec2(x, y) * glm::vec2(lock_x_ ? 0 : 1, lock_y_ ? 0 : 1);
}


void pixel::graphics::Camera::translate(glm::vec2& t)
{
    position_ += t * glm::vec2(lock_x_ ? 0 : 1, lock_y_ ? 0 : 1);
}


void pixel::graphics::Camera::center_at(float x, float y)
{
    position_ = glm::vec2(x, y) - glm::vec2(window_size_) / 2.0f;
}

void pixel::graphics::Camera::center_at(glm::vec2& c)
{
    position_ = c - glm::vec2(window_size_) / 2.0f;
}

glm::mat4 pixel::graphics::Camera::view_matrix()
{
    return glm::mat4();
}
