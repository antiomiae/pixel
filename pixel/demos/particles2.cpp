#include <pixel/pixel.h>
#include <unistd.h>
#include <random>
#include <algorithm>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/random.hpp>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;

const float DT = 1.0 / 60.0f;

default_random_engine generator;
using random_float = uniform_real_distribution<float>;


struct VelocityVerletParticle
{

    glm::vec2 position;
    glm::vec2 last_position;
    glm::vec2 velocity;
    glm::vec2 acc;
    float mass{1.0f};

    glm::vec4 color{1.0, 1.0, 1.0, 1.0};

    VelocityVerletParticle() = default;

    VelocityVerletParticle(glm::vec2 position, glm::vec2 velocity = {0.0, 0.0}, float mass = 1.0,
                           glm::vec4 color = {1.0, 1.0, 1.0, 1.0})
        : position(position),
          velocity(velocity),
          mass(mass),
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
        random_float x_func(pos_range.x, pos_range.z);
        random_float y_func(pos_range.y, pos_range.y);
        random_float v_func(vel_range.x, vel_range.y);
        random_float mass_func(mass_range.x, mass_range.y);

        VelocityVerletParticle p{
            glm::vec2(x_func(generator), y_func(generator)),
            glm::circularRand(1.0f) * v_func(generator),
            mass_func(generator)
        };

        return p;
    }


};


template<class Particle, class AccFunc>
void verlet_step(VelocityVerletParticle& particle, AccFunc& acc, float dt)
{
    auto next_position = (particle.position * 2.0f) - particle.last_position + (dt * dt) * acc(particle);
    particle.last_position = particle.position;
    particle.position = next_position;
}


template<class Particle, class AccFunc>
void velocity_verlet_step(Particle& particle, AccFunc& acc, float dt)
{
    auto next_position = particle.position + particle.velocity * dt + 0.5f * (dt * dt) * particle.acc;
    particle.last_position = particle.position;
    particle.position = next_position;

    auto next_acc = acc(particle);
    particle.velocity = particle.velocity + 0.5f * (particle.acc + next_acc) * dt;
    particle.acc = next_acc;
}


template<class Particle, class AccFunc>
void euler_cromer_step(Particle& particle, AccFunc& acc, float dt)
{
    particle.velocity = particle.velocity + acc(particle) * dt;
    particle.last_position = particle.position;
    particle.position = particle.position + particle.velocity * dt;
}

template<class Particle, class AccFunc>
void euler_step(Particle& particle, AccFunc& acc, float dt)
{
    particle.last_position = particle.position;
    particle.position = particle.position + particle.velocity * dt;
    particle.velocity = particle.velocity + acc(particle) * dt;
}


template<class Vec>
auto vec_min(const Vec& vec)
{
    return min(vec.x, vec.y);
}


template<class Vec>
auto vec_max(const Vec& vec)
{
    return max(vec.x, vec.y);
}


template<class Particle>
void render_particles(
    const vector<Particle>& particles,
    renderers::LineRenderer& renderer,
    const Camera& camera
)
{
    static vector<pair<LineSegment, glm::vec4>> lines;
    lines.clear();

    for (auto& p : particles) {
        auto start = p.position;
        auto stop = p.last_position;

        if (glm::compMax(glm::abs(start - stop)) <= 1.0) {
            stop = start + 1.0f;
        }

        lines.emplace_back(LineSegment{start, stop}, p.color);
    }

    renderer.render(lines, camera);
}


int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
        argv = &argv[2];
        argc -= 2;
    }

    glm::ivec2 virtual_window_size = glm::ivec2{320, 224} * 3;
    glm::ivec2 actual_window_size = virtual_window_size;

    pixel::init(actual_window_size, virtual_window_size);

    Camera camera({0, 0}, {0, 0, 2000, 2000});

    OffscreenRenderTarget render_target{};

    render_target.set_window_size(virtual_window_size);
    camera.set_window_size(render_target.window_size());

    vector<VelocityVerletParticle> particles;

    auto random_particle = [&]() -> auto {
        return VelocityVerletParticle::random_particle(
            glm::vec4{0.f, 0.f, virtual_window_size.x, virtual_window_size.y},
            glm::vec2{100.f, 1000.f},
            glm::vec2{1.f, 1.f}
        );
    };

    {
        glm::vec4 colors[] = {
            {0.0, 1.0, 0.0, 0.1},
            {1.0, 0.0, 1.0, 0.1},
            {0.0, 0.0, 1.0, 0.1},
            {1.0, 1.0, 0.0, 0.1}
        };

        for (auto i = 0u; i < 50; ++i) {
            particles.push_back(random_particle());
            particles.back().color = colors[i % 4];
            particles.back().init_last_position(DT);
        }
    }

    renderers::RendererGroup renderer_group;

    glEnable(GL_BLEND);

    auto max_force = 10000000.0f;
    auto force = max_force;

    auto acc_func = [&](auto& p) -> glm::vec2 {
        auto r = glm::vec2{virtual_window_size} / 2.0f - p.position;
        auto distance = glm::dot(r, r);
        r = glm::normalize(r);

        return (force * p.mass * r / distance);
    };


    pixel::app().set_tick_callback(
        [&] {

            for (auto& p : particles) {
                if (p.position.x < 0 ||
                    p.position.x > virtual_window_size.x ||
                    p.position.y < 0 ||
                    p.position.y > virtual_window_size.y) {

                    auto color = p.color;
                    p = random_particle();
                    p.color = color;
                } else {
                    velocity_verlet_step(p, acc_func, DT);
                }

            }

            glClear(GL_COLOR_BUFFER_BIT);

            render_target.activate();

            //glClear(GL_COLOR_BUFFER_BIT);

            /* do rendering */
            render_particles(particles, renderer_group.get<renderers::LineRenderer>(), camera);
            /* end */

            render_target.deactivate();

            /* blit virtual window to actual window */
            render_target.draw(
                glm::ivec4(
                    0,
                    0,
                    app().render_context().window_size.x,
                    app().render_context().window_size.y
                )
            );
        }
    );

    pixel::app().run();

    return 0;
}
