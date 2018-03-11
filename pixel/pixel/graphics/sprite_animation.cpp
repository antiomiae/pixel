

#include "sprite_animation.h"

namespace pixel::graphics
{

void SpriteAnimation::update(float dt)
{
    timer_ += dt * time_scale_;

    while (timer_ >= current_frame().duration) {
        timer_ -= current_frame().duration;
        advance();
    }
}

void SpriteAnimation::update_sprite(Sprite& sprite)
{
    sprite.texture_region = current_frame().texture_region;
}

bool SpriteAnimation::validate()
{
    return true;
}

SpriteAnimation::Frame SpriteAnimation::current_frame() const
{
    return frames_[current_frame_];
}

void SpriteAnimation::reset()
{
    current_frame_ = 0;
    timer_ = 0;
}

float SpriteAnimation::time_scale()
{
    return time_scale_;
}

void SpriteAnimation::set_time_scale(float ts)
{
    time_scale_ = ts;
}

void SpriteAnimation::advance()
{
    ++current_frame_;
    if (current_frame_ >= frames_.size()) {
        current_frame_ = 0;
    }
}

void SpriteAnimation::add_frame(const SpriteAnimation::Frame& f)
{
    frames_.push_back(f);
}

void SpriteAnimation::add_frame(const TextureRegion& t, float duration)
{
    frames_.emplace_back(t, duration);
}

SpriteAnimation SpriteAnimation::copy() const
{
    return *this;
}

}
