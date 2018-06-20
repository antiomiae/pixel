#include <pixel/pixel.h>
#include <pixel/physics/physics.h>
#include <unistd.h>
#include <algorithm>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/color_space.hpp>
#include <random>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::physics;
using namespace pixel::input;

const float DT = 1.0 / 60.0f;

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
            glm::vec2{10.f, 1000.f},
            glm::vec2{1.f, 1.f}
        );
    };

    auto random_color = []() -> auto {
        static default_random_engine generator;
        static std::uniform_real_distribution<float> h_rand(0.f, 360.f);

        return hsv_color(h_rand(generator), 0.8f, 0.8f);
    };


    for (auto i = 0u; i < 50; ++i) {
        particles.push_back(random_particle());
        particles.back().color = random_color();
        particles.back().init_last_position(DT);
    }

    renderers::RendererGroup renderer_group;

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

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

                    p = random_particle();
                    p.color = random_color();
                    p.init_last_position(DT);
                } else {
                    IntegrationMethods::velocity_verlet_step(p, acc_func, DT);
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
