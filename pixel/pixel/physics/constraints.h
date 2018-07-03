#ifndef PIXEL_MAIN_CONSTRAINTS_H
#define PIXEL_MAIN_CONSTRAINTS_H

#include <pixel/math.h>
#include "particle.h"

namespace pixel::physics
{

struct Spring
{
    Spring() = default;

    Spring(float k)
        : k(k)
    { };

    float k = 1.0f; // force per unit length
    float b = 0.0f;
    float min = 0.0f;

    glm::vec2 force(const Particle& p1, const Particle& p2)
    {
        auto r = p2.position - p1.position;
        auto v = p1.velocity - p2.velocity;
        auto d = glm::length(r);
        r = glm::normalize(r);

        return force(d) * r - b * v;
    }

    glm::vec2 force(glm::vec2 b1, glm::vec2 b2)
    {
        auto r = b2 - b1;
        auto d = glm::length(r);
        r = glm::normalize(r);

        return force(d) * r;
    }

    float force(float d) {
        return k * (d - min);
    }
};

}

#endif //PIXEL_MAIN_CONSTRAINTS_H
