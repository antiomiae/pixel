#ifndef PIXEL_SPRITE_ANIMATION_H
#define PIXEL_SPRITE_ANIMATION_H

#include <vector>
#include "sprite.h"

namespace pixel::graphics
{

using namespace std;

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

private:
    vector<Frame> frames_{};
    unsigned current_frame_;
    float timer_;
    float time_scale_{1.0};
};

};

#endif //PIXEL_SPRITE_ANIMATION_H
