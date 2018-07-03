#include <pixel/pixel.h>
#include <pixel/physics/physics.h>
#include <pixel/physics/particle.h>
#include <pixel/physics/constraints.h>
#include <unistd.h>
#include <algorithm>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/color_space.hpp>
#include <random>
#include <glm/gtx/fast_square_root.hpp>
#include <chrono>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::physics;
using namespace pixel::input;


using LineSegmentBatch = vector<LineSegment>;

class Level;


struct ParticleSpringSystem
{
    Particle* p1{};
    Particle* p2{};

    Spring spring{};

    void update(float dt)
    {
        auto force = spring.force(*p1, *p2);

        p1->velocity += dt * force * p1->inverse_mass;
        p2->velocity -= dt * force * p2->inverse_mass;
    }

    void render(LineSegmentBatch& batch)
    {
        if (p1 != nullptr && p2 != nullptr) {
            /* draw p1 */
            batch.push_back(LineSegment(p1->position, p1->position + glm::vec2(1, 1)));

            /* draw p2 */
            batch.push_back(LineSegment(p2->position, p2->position + glm::vec2(1, 1)));

            /* draw spring */
            batch.push_back(LineSegment(p1->position, p2->position));
        }
    }
};


class Level
{
public:
    Level(glm::ivec2 window_size)
        : window_size_(window_size)
    {
        camera_.set_window_size(window_size);
    }

    Camera& camera()
    {
        return camera_;
    }

    renderers::RendererGroup& renderer_group()
    {
        return renderer_group_;
    }

    void load_sprites(vector<string> const& paths)
    {
        texture_atlas_.start_batch();
        for_each(begin(paths), end(paths), [&](auto& path) { texture_atlas_.add_image(path); });
        texture_atlas_.stop_batch();

        cout << texture_atlas_.debug_print() << endl;

        sprite_texture_ = texture_atlas_.as_texture();
    }

    Sprite get_sprite(string const& path)
    {
        Sprite s;
        s.texture_region = texture_atlas_.lookup(path);
        return s;
    }


    Texture& sprite_texture()
    {
        return sprite_texture_;
    }


private:
    glm::ivec2 window_size_;

    Camera camera_;

    renderers::RendererGroup renderer_group_;

    TextureAtlas texture_atlas_;
    Texture sprite_texture_;
};


struct Physics
{
    Physics() {
        particles.reserve(100);
        springs.reserve(1000);
        lines.reserve(1000);
    }

    Particle& add_particle()
    {
        auto& p = particles.emplace_back();
        return p;
    }

    LineSegment& add_line()
    {
        return lines.emplace_back();
    }

    ParticleSpringSystem& join_particles(Particle& p1, Particle& p2)
    {
        auto& s = springs.emplace_back();
        s.spring.min = glm::distance(p1.position, p2.position);
        s.p1 = &p1;
        s.p2 = &p2;

        return s;
    }

    void update(float dt)
    {
        for (auto& s : springs) {
            s.update(dt);
        }

        for (auto& p : particles) {
            p.step(dt);
        }
    }

    void render(LineSegmentBatch& batch)
    {
        for (auto& s : springs) {
            s.render(batch);
        }

        std::for_each(begin(lines), end(lines), [&] (auto l){ batch.push_back(l); });
    }

    vector<Particle> particles;
    vector<ParticleSpringSystem> springs;
    vector<LineSegment> lines;
};


void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{640, 400};
    glm::ivec2 actual_window_size = virtual_window_size * 2;

    pixel::init(actual_window_size, virtual_window_size, argc, argv);

    OffscreenRenderTarget render_target{};
    render_target.set_window_size(virtual_window_size);

    Level level(virtual_window_size);

    Physics physics;

    auto& p1 = physics.add_particle();
    auto& p2 = physics.add_particle();
    auto& p3 = physics.add_particle();

    p1.position = glm::vec2(virtual_window_size / 2) - glm::vec2(50, 0);
    p2.position = glm::vec2(virtual_window_size / 2) + glm::vec2(50, 0);
    p3.position = glm::vec2(virtual_window_size / 2) + glm::vec2(75, 50);

    p1.velocity.y += 50;
    p2.velocity.y -= 50;

    p1.set_mass(10);
    p2.set_mass(10);
    p3.set_mass(0.1);

    auto& spring1 = physics.join_particles(p1, p2);

    spring1.spring.k = 10;
    spring1.spring.b = 0.1;

    auto& spring2 = physics.join_particles(p2, p3);

    spring2.spring.k = 100;
    spring2.spring.b = 0.2;

    {
        auto& floor = physics.add_line();
        floor.p0 = {0, 50.5};
        floor.p1 = {virtual_window_size.x, 50.5};
    }

    auto update = [&]
    {
        physics.update(1/60.0f);
    };

    auto render = [&]
    {
        static LineSegmentBatch lines;

        lines.clear();

        physics.render(lines);

        level.renderer_group().get<renderers::LineRenderer>().render(lines, level.camera());
    };

    glEnable(GL_LINE_SMOOTH);

    pixel::app().set_tick_callback(
        [&] {

            update();

            glClear(GL_COLOR_BUFFER_BIT);

            render_target.activate();

            glClear(GL_COLOR_BUFFER_BIT);

            /* do rendering */
            render();
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

}


int main(int argc, char* argv[])
{
   start(argc, argv);

   return 0;
}
