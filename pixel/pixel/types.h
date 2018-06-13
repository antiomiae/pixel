

#ifndef PIXEL_TYPES_H
#define PIXEL_TYPES_H

#include <cstdint>
#include <utility>
#include <ostream>
#include "math.h"

namespace pixel
{

using namespace std;

using uint = unsigned int;


/**
 * Generic rectangle class
 * \tparam T numeric type for position and size fields
 */
template<typename T>
class Rect
{

public:

    using Type = T;

    Type x, y, w, h;

    pair<Type, Type> position()
    {
        return {x, y};
    }
};

/**
 * int Rect specialization
 */
using RectI = Rect<int>;
/**
 * float Rect specialization
 */
using RectF = Rect<float>;

template<class RectType>
glm::vec2 position(const RectType& r)
{
    return {r.x, r.y};
}

template<class RectType>
glm::vec2 size(const RectType& r)
{
    return {r.w, r.h};
}

/**
 * Simple data structure for representing line segments
 */
class LineSegment
{

public:
    float x0, y0, x1, y1;

    LineSegment() = default;

    LineSegment(float x0, float y0, float x1, float y1)
        : x0(x0),
          y0(y0),
          x1(x1),
          y1(y1)
    { }

    LineSegment(glm::vec2 p0, glm::vec2 p1)
        : x0(p0.x),
          y0(p0.y),
          x1(p1.x),
          y1(p1.y)
    { }

    float length()
    {
        return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
    }

    friend ostream& operator<<(ostream& stream, const LineSegment& line_segment)
    {
        stream << "[("
               << line_segment.x0 << ", " << line_segment.y0
               << "), ("
               << line_segment.x1 << ", " << line_segment.y1
               << ")]";

        return stream;
    }
};

};

#endif
