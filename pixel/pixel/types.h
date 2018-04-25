

#ifndef PIXEL_TYPES_H
#define PIXEL_TYPES_H

#include <cstdint>
#include <utility>
#include "math.h"

namespace pixel
{

using namespace std;

using uint = unsigned int;

template <typename T> class Rect
{

public:

    using Type = T;

    Type x, y, w, h;

    pair<Type, Type> position()
    {
        return {x, y};
    }
};

using RectI = Rect<int>;
using RectF = Rect<float>;

};

#endif //PIXEL_TYPES_H
