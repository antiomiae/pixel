#ifndef PIXEL_SPRITE_BATCH_H
#define PIXEL_SPRITE_BATCH_H


#include <vector>
#include "sprite.h"


namespace pixel::graphics
{

using namespace std;

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


#endif //PIXEL_SPRITE_BATCH_H
