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

const float DT = 1.0 / 120.0f;

default_random_engine generator;
using random_float = uniform_real_distribution<float>;

struct VerletParticle
{
public:
    glm::vec2 position;
    glm::vec2 last_position;
    glm::vec4 color{1.0, 1.0, 1.0, 1.0};
    float mass;

    VerletParticle() = default;

    VerletParticle(glm::vec2 position, float mass)
        : position(position),
          last_position(position),
          mass(mass)
    { };
};

struct VelocityVerletParticle : VerletParticle
{
    glm::vec2 velocity;
    glm::vec2 acc;

    VelocityVerletParticle() = default;

    VelocityVerletParticle(glm::vec2 position, glm::vec2 velocity = {0.0, 0.0}, float mass = 1.0)
        : VerletParticle(position, mass),
          velocity(velocity)
    {
    }

    static VelocityVerletParticle
    random_particle(glm::vec2 lower_left, glm::vec2 upper_right, float max_velocity, glm::vec2 mass_range)
    {
        random_float x_func(lower_left.x, upper_right.x);
        random_float y_func(lower_left.y, upper_right.y);
        random_float v_func(-max_velocity, max_velocity);
        random_float mass_func(mass_range.x, mass_range.y);

        VelocityVerletParticle p{
            glm::vec2(x_func(generator), y_func(generator)),
            glm::circularRand(max_velocity),
            mass_func(generator)
        };

        return p;
    }
};


void verlet_step(VerletParticle& particle, glm::vec2 acc, float dt)
{
    auto next_position = (particle.position * 2.0f) - particle.last_position + (dt * dt) * acc;
    particle.last_position = particle.position;
    particle.position = next_position;
}


void velocity_verlet_step(VelocityVerletParticle& particle, function<glm::vec2(VelocityVerletParticle&)> acc, float dt)
{
    auto next_position = particle.position + particle.velocity * dt + 0.5f * (dt * dt) * particle.acc;
    particle.last_position = particle.position;
    particle.position = next_position;

    auto next_acc = acc(particle);
    particle.velocity = particle.velocity + 0.5f * (particle.acc + next_acc) * dt;
    particle.acc = next_acc;
}

void euler_cromer_step(VelocityVerletParticle& particle, function<glm::vec2(VelocityVerletParticle&)> acc, float dt)
{
    particle.velocity = particle.velocity + acc(particle) * dt;
    particle.last_position = particle.position;
    particle.position = particle.position + particle.velocity * dt;
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


VerletParticle
random_particle(glm::vec2 lower_left, glm::vec2 upper_right, float max_velocity, glm::vec2 mass_range, float dt)
{
    random_float x_func(lower_left.x, upper_right.x);
    random_float y_func(lower_left.y, upper_right.y);
    random_float v_func(-max_velocity, max_velocity);
    random_float mass_func(mass_range.x, mass_range.y);

    VerletParticle p{glm::vec2(x_func(generator), y_func(generator)), mass_func(generator)};
    p.last_position += -glm::vec2(v_func(generator), v_func(generator)) * dt;

    return p;
}


void render_particles(const vector<VerletParticle>& particles, renderers::LineRenderer& renderer, const Camera& camera)
{
    static vector<LineSegment> lines;
    lines.clear();

    for (auto& p : particles) {
        auto start = p.position;
        auto stop = p.last_position;

        if (glm::compMax(glm::abs(start - stop)) <= 1.0) {
            stop = start + 1.0f;
        }

        lines.emplace_back(start, stop);
    }

    renderer.render(lines, camera);
}


void render_particles(
    const vector<VelocityVerletParticle>& particles,
    renderers::LineRenderer& renderer,
    const Camera& camera
)
{
    static vector<LineSegment> lines;
    lines.clear();

    for (auto& p : particles) {
        auto start = p.position;
        auto stop = p.last_position;

        if (glm::compMax(glm::abs(start - stop)) <= 1.0) {
            stop = start + 1.0f;
        }

        lines.emplace_back(start, stop);
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

    camera.set_window_size(virtual_window_size);

    vector<VelocityVerletParticle> particles{100};

    for (auto i = 0u; i < 10; ++i) {
        particles.push_back(
            VelocityVerletParticle::random_particle({0.0, 0.0}, virtual_window_size, 100.0f, {1.0, 100.0})
        );
    }

    renderers::RendererGroup renderer_group;

    pixel::app().set_tick_callback(
        [&] {
            static float theta = 0.0;

            theta += 0.1;

            //auto force = glm::vec2{cos(theta), sin(theta)} * 2.0f;
            auto max_force = 1000.0f;
            //auto force = cos(theta) * max_force/2.0f + max_force/2.0f;
            auto force = max_force;

            auto acc_func = [=](auto& p) -> auto {
                auto r = glm::vec2{virtual_window_size} / 2.0f - p.position;
                auto distance_squared = sqrtf(glm::dot(r, r));
                r = glm::normalize(r);

                return (force * p.mass * r / distance_squared);
            };

            for (auto& p : particles) {
                if (p.position.x < 0 ||
                    p.position.x > virtual_window_size.x ||
                    p.position.y < 0 ||
                    p.position.y > virtual_window_size.y) {

                    p = VelocityVerletParticle::random_particle({0.0, 0.0}, virtual_window_size, 100.0f, {1.0, 100.0});
                } else {
                    euler_cromer_step(p, acc_func, DT);
                }

            }

            /* do rendering */
            render_particles(particles, renderer_group.get<renderers::LineRenderer>(), camera);
            /* end */

        }
    );

    pixel::app().run();

    return 0;
}
