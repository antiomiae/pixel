#include <pixel/pixel.h>
#include <unistd.h>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;


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


void verlet_step(VerletParticle& particle, float acc, float dt)
{
    auto next_position = (particle.position * 2.0f) - particle.last_position + (dt * dt) * acc;
    particle.last_position = particle.position;
    particle.position = next_position;
}


int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
        argv = &argv[2];
        argc -= 2;
    }

    glm::ivec2 virtual_window_size = {320, 224};
    glm::ivec2 actual_window_size = virtual_window_size * 3;

    pixel::init(actual_window_size, virtual_window_size);

    Camera camera({0, 0}, {0, 0, 2000, 2000});

    OffscreenRenderTarget render_target{};

    render_target.set_window_size(virtual_window_size);
    camera.set_window_size(render_target.window_size());

    pixel::app().set_tick_callback(
        [&] {
            render_target.activate();

            glClear(GL_COLOR_BUFFER_BIT);

            /* do rendering */

            /* end */

            render_target.deactivate();

            glClear(GL_COLOR_BUFFER_BIT);

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
