#ifndef PIXEL_MAIN_PHYSICS_H
#define PIXEL_MAIN_PHYSICS_H

#include <random>
#include <pixel/random.h>
#include <glm/gtc/random.hpp>

namespace pixel::physics
{

struct VelocityVerletParticle
{
    glm::vec2 position;
    glm::vec2 last_position;
    glm::vec2 velocity;
    glm::vec2 acc;
    float mass{1.0f};
    float inv_mass{1.0f};

    glm::vec4 color{1.0, 1.0, 1.0, 1.0};

    VelocityVerletParticle() = default;

    VelocityVerletParticle(
        glm::vec2 position, glm::vec2 velocity = {0.0, 0.0}, float mass = 1.0,
        glm::vec4 color = {1.0, 1.0, 1.0, 1.0}
    )
        : position(position),
          velocity(velocity),
          mass(mass),
          inv_mass(1.0f/mass),
          color(color)
    {
    }

    void init_last_position(float dt)
    {
        last_position = position - dt * velocity;
    }

    static VelocityVerletParticle
    random_particle(glm::vec4 pos_range, glm::vec2 vel_range, glm::vec2 mass_range)
    {
        auto& generator = pixel::random::default_engine();

        using random_float = std::uniform_real_distribution<float>;

        random_float x_func(pos_range.x, pos_range.z);
        random_float y_func(pos_range.y, pos_range.w);
        random_float v_func(vel_range.x, vel_range.y);
        random_float mass_func(mass_range.x, mass_range.y);

        return {
            glm::vec2(x_func(generator), y_func(generator)),
            glm::circularRand(1.0f) * v_func(generator),
            mass_func(generator)
        };
    }

};


struct IntegrationMethods
{
    template<class Particle, class AccFunc>
    static void verlet_step(Particle& particle, AccFunc& acc, float dt)
    {
        auto next_position = (particle.position * 2.0f) - particle.last_position + (dt * dt) * acc(particle);
        particle.last_position = particle.position;
        particle.position = next_position;
    }


    template<class Particle, class AccFunc>
    static void velocity_verlet_step(Particle& particle, AccFunc& acc, float dt)
    {
        auto next_position = particle.position + particle.velocity * dt + 0.5f * (dt * dt) * particle.acc;
        particle.last_position = particle.position;
        particle.position = next_position;

        auto next_acc = acc(particle);
        particle.velocity = particle.velocity + 0.5f * (particle.acc + next_acc) * dt;
        particle.acc = next_acc;
    }


    template<class Particle, class AccFunc>
    static void euler_cromer_step(Particle& particle, AccFunc& acc, float dt)
    {
        particle.velocity = particle.velocity + acc(particle) * dt;
        particle.last_position = particle.position;
        particle.position = particle.position + particle.velocity * dt;
    }


    template<class Particle, class AccFunc>
    static void euler_step(Particle& particle, AccFunc& acc, float dt)
    {
        particle.last_position = particle.position;
        particle.position = particle.position + particle.velocity * dt;
        particle.velocity = particle.velocity + acc(particle) * dt;
    }
};

}

#endif //PIXEL_MAIN_PHYSICS_H
