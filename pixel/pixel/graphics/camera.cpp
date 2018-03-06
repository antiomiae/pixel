#include <iostream>
#include "camera.h"

namespace pixel::graphics
{

using namespace std;

Camera::Camera(glm::ivec2 window_size, glm::vec4 bounds)
    : window_size_{window_size},
      bounds_{bounds}
{
}

void Camera::lock_x(bool lock)
{
    lock_x_ = lock;
}

void Camera::lock_y(bool lock)
{
    lock_y_ = lock;
}

void Camera::translate(float x, float y)
{
    position_ += glm::vec2(x, y) * glm::vec2(lock_x_ ? 0 : 1, lock_y_ ? 0 : 1);
}


void Camera::translate(const glm::vec2& t)
{
    position_ += t * glm::vec2(lock_x_ ? 0 : 1, lock_y_ ? 0 : 1);
}


void Camera::center_at(float x, float y)
{
    position_ = glm::vec2(x, y) - glm::vec2(window_size_) / 2.0f;
}

void Camera::center_at(const glm::vec2& c)
{
    position_ = c - glm::vec2(window_size_) / 2.0f;
}

glm::mat4 Camera::view_matrix()
{
    auto center = glm::vec2(window_size_) / 2.0f;
    auto v = glm::mat4();
    /* translate center of view rect to 0,0 */
    v = glm::translate(v, -glm::vec3(position_ + center, 0.0));

    /* scale */
    v = glm::scale(glm::mat4(), glm::vec3(scale_, 1.0)) * v;

    /* translate scaled contents back to center of view rect */
    v = glm::translate(glm::mat4(), glm::vec3(center, 0.0)) * v;

    return v;
}

glm::vec2 Camera::position()
{
    return position_;
}

glm::ivec2 Camera::window_size()
{
    return window_size_;
}

glm::vec2 Camera::scale()
{
    return scale_;
}

void Camera::position_at(float x, float y)
{
    position_ = {x, y};
}

void Camera::position_at(const glm::vec2& new_pos)
{
    position_ = new_pos;
}

void Camera::scale_by(float dsx, float dsy)
{
    scale_ *= glm::vec2(dsx, dsy);
}

void Camera::scale_by(const glm::vec2& s)
{
    scale_ *= s;
}

void Camera::scale(float s)
{
    scale_ = {s, s};
}

void Camera::scale(float x, float y)
{
    scale_ = {x, y};
}

void Camera::scale(const glm::vec2& s)
{
    scale_ = s;
}


void Camera::set_window_size(const glm::vec2& v)
{
    window_size_ = v;
}

/**
 * The projected region described by this camera's properties
 * \return vec4(x0, y0, x1, y1) a vector of the lower-left and upper-right coordinates of the view rect
 */
glm::vec4 Camera::view_rect()
{
    auto o = glm::vec4(0.0, 0.0, 0.0, 1.0);
    auto m = glm::vec4(glm::vec2(window_size_), 0.0, 1.0);
    auto ivm = glm::inverse(view_matrix());

    o = ivm * o;
    m = ivm * m;

    return glm::vec4(o.x, o.y, m.x, m.y);
}

glm::mat4 Camera::projection_matrix()
{
    return glm::ortho(0.0f, (float)window_size_.x, 0.0f, (float)window_size_.y);
}

};
