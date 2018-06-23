#include <pixel/pixel.h>
#include <pixel/physics/physics.h>
#include <unistd.h>
#include <algorithm>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/color_space.hpp>
#include <random>
#include <glm/gtx/fast_square_root.hpp>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::physics;
using namespace pixel::input;

const float DT = 1.0 / 120.0f;

using ParticleType = VelocityVerletParticle;

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


glm::vec4 hsv_color(glm::vec4 hsva)
{
    return glm::vec4(glm::rgbColor(glm::vec3{hsva}), hsva.a);
}

glm::vec4 hsv_color(float h, float s, float v)
{
    return hsv_color(glm::vec4{h, s, v, 1.0f});
}

glm::vec4 hsv_color(glm::vec3 hsv)
{
    return hsv_color(glm::vec4{hsv, 1.0f});
}



class ParticleController
{
public:

    using StepFunction = std::function<void(vector<ParticleType>&,float,float)>;

    ParticleController(uint num_particles, glm::vec4 field_size)
        : field_size_(field_size)
    {
        for (auto i = 0u; i < num_particles; ++i) {
            particles_.push_back(random_particle());
            particles_.back().color = random_color();
            particles_.back().init_last_position(DT);
        }
    }

    VelocityVerletParticle random_particle()
    {
        return VelocityVerletParticle::random_particle(
            field_size_,
            glm::vec2{0, 0},
            glm::vec2{1.f, 1000.f}
        );
    };

    glm::vec4 random_color()
    {
        static default_random_engine generator;
        static std::uniform_real_distribution<float> h_rand(0.f, 360.f);

        auto c = hsv_color(h_rand(generator), 0.8f, 0.8f);
        c.a = 0.2f;
        return c;
    };

    void step(float dt) {
        if (step_callback_) {
            step_callback_(particles_, 1.f, dt);
        }
    }

    void set_step_callback(StepFunction cb)
    {
        step_callback_ = std::move(cb);
    }

    vector<ParticleType>& particles()
    {
        return particles_;
    }

private:
    glm::vec4 field_size_;

    vector<ParticleType> particles_;

    StepFunction step_callback_;

};


struct MutualGravitySimulation
{
    static void step(vector<ParticleType>& bodies, float dt, float gravity)
    {
        for (auto& particle : bodies) {
            auto next_position = particle.position + particle.velocity * dt + 0.5f * (dt * dt) * particle.acc;
            particle.last_position = particle.position;
            particle.position = next_position;
        }


        for (auto i = 0u; i < bodies.size(); ++i) {
            auto& p_0 = bodies[i];

            glm::vec2 total_force{0};

            for (auto j = 0u; j < bodies.size(); ++j) {
                if (i == j) {
                    continue;
                }

                auto p_n = bodies[j];

                auto r = p_n.position - p_0.position;

                auto d = glm::fastLength(r);

                total_force += gravity * p_n.mass * p_0.mass * r / powf(d + 1.0f, 3.0);
            }

            p_0.velocity = p_0.velocity + 0.5f * (p_0.acc + total_force * p_0.inv_mass) * dt;
        }
    }
};


int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
        argv = &argv[2];
        argc -= 2;
    }

    glm::ivec2 virtual_window_size = glm::vec2{1440, 900};
    glm::ivec2 actual_window_size = virtual_window_size;

    pixel::init(actual_window_size, virtual_window_size);

    Camera camera({0, 0}, {0, 0, 2000, 2000});

    OffscreenRenderTarget render_target{};

    render_target.set_window_size(virtual_window_size);
    camera.set_window_size(render_target.window_size());

    ParticleController particle_controller{500, glm::vec4(0.f, 0.f, virtual_window_size)};

    renderers::RendererGroup renderer_group;

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    particle_controller.set_step_callback(&MutualGravitySimulation::step);

    pixel::app().set_tick_callback(
        [&] {
            particle_controller.step(DT);

            glClear(GL_COLOR_BUFFER_BIT);

            render_target.activate();

            //glClear(GL_COLOR_BUFFER_BIT);

            /* do rendering */
            render_particles(particle_controller.particles(), renderer_group.get<renderers::LineRenderer>(), camera);
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
