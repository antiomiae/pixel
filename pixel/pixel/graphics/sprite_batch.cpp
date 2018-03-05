

#include "sprite_batch.h"

namespace pixel::graphics {

void SpriteBatch::restart()
{
    sprites_.clear();
}

void SpriteBatch::add(Sprite s)
{
    sprites_.push_back(s);
}

std::vector<Sprite>& SpriteBatch::sprites()
{
    return sprites_;
}

};
