#ifndef PIXEL_SPRITE_H
#define PIXEL_SPRITE_H

#include "common.h"

namespace pixel::graphics
{

class Sprite
{
public:
    Sprite() = default;

    /**
        * Sprite orientation flags
        * */
    enum FlipFlag : uint8_t
    {
        kHorizontal = 0x8,
        kVertical = 0x4,
        kDiagonal = 0x2
    };

    union
    {
        glm::vec2 position{};
        struct
        {
            float x, y;
        };
    };

    glm::vec2 center{0.f, 0.f};

    glm::vec4 color{1.0, 1.0, 1.0, 1.0};

    glm::vec4 tint{0.0, 0.0, 0.0, 0.0};

    float angle{0.f};

    uint32_t flip_flags{0};

    TextureRegion texture_region;

    void flip_h(bool);
    bool flip_h();

    void flip_v(bool);
    bool flip_v();

    void flip_d(bool);
    bool flip_d();
};


/**
 * Animates a sprite by updating its image properties over time
 */
class SpriteAnimation
{
public:
    struct Frame
    {
        TextureRegion texture_region{};
        float duration{0};

        Frame() = default;

        Frame(TextureRegion tr, float d)
            : texture_region{tr},
              duration{d}
        { }
    };

    SpriteAnimation() = default;

    void add_frame(const Frame& f);
    void add_frame(const TextureRegion& t, float duration);

    void update(float dt);
    void update_sprite(Sprite& sprite);
    bool validate();
    Frame current_frame() const;
    void reset();

    float time_scale();
    void set_time_scale(float);

    void advance();

    SpriteAnimation copy() const;

private:
    vector<Frame> frames_{};
    unsigned current_frame_;
    float timer_;
    float time_scale_{1.0};
};


class SpriteBatch
{

public:

    SpriteBatch() = default;

    void restart();
    void add(const Sprite& s);

    vector<Sprite>& sprites();

private:

    vector<Sprite> sprites_;
};

};
#endif //PIXEL_SPRITE_H
