

#ifndef PIXEL_MAIN_PARTICLE_H
#define PIXEL_MAIN_PARTICLE_H

#include <pixel/math.h>

namespace pixel::physics
{

struct Particle
{
    Particle() = default;

    Particle(glm::vec2 p, glm::vec2 v, float m)
        : position(p),
          last_position(p),
          velocity(v)
    {
        set_mass(m);
    }

    void step(float dt)
    {
        last_position = position;
        position += dt * velocity;
    }

    glm::vec2 position{};
    glm::vec2 last_position{};
    glm::vec2 velocity{};
    //glm::vec2 acceleration; // I'm okay with this

    float mass{1.0f};
    float inverse_mass{1.0f};

    void set_mass(float m)
    {
        mass = m;
        inverse_mass = 1.0f / m;
    }


};

}

#endif //PIXEL_MAIN_PARTICLE_H
