#include <iostream>
#include "camera.h"

namespace pixel::graphics
{

using namespace std;

Camera::Camera(const glm::ivec2& window_size, const glm::vec4& bounds)
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
    position_ = glm::vec2(x, y);
}

void Camera::center_at(const glm::vec2& c)
{
    position_ = c;
}

glm::mat4 Camera::view_matrix()
{
    return parallax_view_matrix({1.0, 1.0});
}

glm::mat4 Camera::parallax_view_matrix(const glm::vec2& parallax_scale)
{
    auto center = glm::vec2(window_size_) / 2.0f;
    auto scaled_center = center / scale_;
    auto v = glm::mat4();

    /* move to account for camera position */
    v = glm::translate(v, -glm::vec3(glm::floor(position_ * parallax_scale), 0.0));
    /* translate center of view rect to 0,0 */
    v = glm::translate(glm::mat4(), -glm::vec3(center, 0.0)) * v;
    /* scale */
    auto local_scale = scale_;
    if (parallax_scale == glm::vec2{0.0, 0.0}) {
        local_scale = {1.0, 1.0};
    }

    v = glm::rotate(glm::mat4(), angle_, glm::vec3(0.0f, 0.0f, 1.0f)) * v;

    v = glm::scale(glm::mat4(), glm::vec3(local_scale, 1.0)) * v;

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
    position_at({x, y});
}

void Camera::position_at(const glm::vec2& v)
{
    position_ = v - (glm::vec2(window_size_) / scale_ / 2.0f);
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


void Camera::set_window_size(const glm::ivec2& v)
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
    return glm::ortho(0.0f, (float) window_size_.x, 0.0f, (float) window_size_.y);
}

void Camera::set_window_size(int w, int h)
{
    set_window_size({w, h});
}

void Camera::follow(float x, float y)
{
    position_ = glm::vec2(
        lock_x_ ? position_.x : x - (float) window_size_.x / 2.0,
        lock_y_ ? position_.y : y - (float) window_size_.y / 2.0
    );
}

void Camera::follow(const glm::vec2& o)
{
    follow(o.x, o.y);
}

void Camera::set_angle(float a)
{
    angle_ = a;
}

};
