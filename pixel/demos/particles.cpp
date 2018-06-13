#include <pixel/pixel.h>
#include <unistd.h>
#include <random>
#include <glm/gtx/component_wise.hpp>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;


default_random_engine generator;


struct VerletParticle
{
    glm::vec2 position;
    glm::vec2 last_position;
    float mass;

    VerletParticle() = default;

    VerletParticle(glm::vec2 position, float mass)
        : position(position),
          last_position(position),
          mass(mass)
    { };
};


void verlet_step(VerletParticle& particle, glm::vec2 acc, float dt)
{
    auto next_position = (particle.position * 2.0f) - particle.last_position + (dt * dt) * acc;
    particle.last_position = particle.position;
    particle.position = next_position;
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

VerletParticle random_particle(glm::vec2 lower_left, glm::vec2 upper_right)
{
    uniform_real_distribution<float> x_func(lower_left.x, upper_right.x);
    uniform_real_distribution<float> y_func(lower_left.y, upper_right.y);

    return VerletParticle{glm::vec2(x_func(generator), y_func(generator)), 1.0};
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


    vector<VerletParticle> particles{100};

    for (auto i = 0u; i < 10000; ++i) {
        particles.push_back(random_particle({0.0, 0.0}, virtual_window_size));
    }

    renderers::RendererGroup renderer_group;

    pixel::app().set_tick_callback(
        [&] {
            static float theta = 0.0;

            theta += 0.1;

            //auto force = glm::vec2{cos(theta), sin(theta)} * 2.0f;
            auto force = cos(theta) * 50.0f + 50.0f;

            for (auto& p : particles) {
                auto towards_center = glm::vec2{virtual_window_size} / 2.0f - p.position;
                if (towards_center.x == 0.0) {
                    towards_center.x = 1.0;
                }
                if (towards_center.y == 0.0) {
                    towards_center.y = 1.0;
                }

                auto local_force = (force / towards_center);

                verlet_step(p, local_force, 1.0f / 60.0f);
            }


            glClear(GL_COLOR_BUFFER_BIT);


            //render_target.activate();

            //glClear(GL_COLOR_BUFFER_BIT);

            /* do rendering */
            render_particles(particles, renderer_group.get<renderers::LineRenderer>(), camera);
            /* end */

            //render_target.deactivate();



            /* blit virtual window to actual window */
//            render_target.draw(
//                glm::ivec4(
//                    0,
//                    0,
//                    app().render_context().window_size.x,
//                    app().render_context().window_size.y
//                )
//            );
        }
    );

    pixel::app().run();

    return 0;
}
