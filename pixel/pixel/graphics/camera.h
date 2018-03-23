#ifndef PIXEL_CAMERA_H
#define PIXEL_CAMERA_H

#include <pixel/math/math.h>

namespace pixel::graphics
{

class Camera
{
public:
    Camera(const glm::ivec2& window_size, const glm::vec4& bounds);
    Camera() = default;

    void lock_x(bool lock = true);
    void lock_y(bool lock = true);

    void translate(float x, float y);
    void translate(const glm::vec2&);

    /**
     * Place camera so that (x, y) is at center of frame
     * \param x
     * \param y
     */
    void center_at(float x, float y);
    void center_at(const glm::vec2&);

    /**
     * Place camera so that (x, y) is at center of frame, respecting camera axis locks.
     * \param x
     * \param y
     */
    void follow(float x, float y);
    void follow(const glm::vec2&);

    void position_at(float x, float y);
    void position_at(const glm::vec2&);

    void scale_by(float dsx, float dsy);
    void scale_by(const glm::vec2&);

    void scale(float s);
    void scale(float x, float y);
    void scale(const glm::vec2&);

    void set_window_size(const glm::ivec2& v);
    void set_window_size(int w, int h);

    void set_angle(float);

    glm::mat4 view_matrix();
    glm::mat4 parallax_view_matrix(const glm::vec2&);
    glm::mat4 projection_matrix();

    glm::vec2 position();
    glm::vec2 scale();
    glm::ivec2 window_size();
    glm::vec4 view_rect();

private:
    bool lock_x_{false};
    bool lock_y_{false};

    glm::ivec2 window_size_{320, 240};
    glm::vec4 bounds_{0, 0, 512, 512};
    glm::vec2 position_{0.0, 0.0};
    glm::vec2 scale_{1.0};
    float angle_{0.0};
};

};

#endif //PIXEL_CAMERA_H
