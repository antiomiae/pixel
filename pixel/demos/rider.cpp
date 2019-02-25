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

const float GRAVITY = 100.0f;

class Level;



float cross_2d(glm::vec2 a, glm::vec2 b)
{
    return a.x * b.y - a.y * b.x;
}


struct Intersection
{
    static float time_of_intersection(LineSegment& line_segment, Particle& particle)
    {
        /* Basic formula:
         * q s -> x v
         * t = (x − p) × v / (r × v)
         * where v × w = vx wy - vy wx
         */

        auto r = line_segment.p1 - line_segment.p0;
        auto p = line_segment.p0;

        auto x = particle.last_position;
        auto v = particle.position - x;

        /* solving for t, particle time */
        auto t_num = cross_2d((x - p), r);

        /* solve for u, linesegment parameter */
        auto u_num = cross_2d((x - p), v);

        auto denom = cross_2d(r, v);

        if (denom == 0.0f) {
            return -1.0f;
        } else {
            float t = t_num / denom;
            float u = u_num / denom;

            if (t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f) {
                return t;
            } else {
                return -1.0f;
            }
        }
    }
};

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
            p.velocity.y -= dt * GRAVITY;
            p.step(dt);

            float last_collision_time = 1.0f;
            glm::vec2 normal;

            for (auto& l : lines) {
                auto t = Intersection::time_of_intersection(l, p);

                if (t >= 0.0f && t < last_collision_time) {
                    last_collision_time = t;
                    auto n = glm::normalize(l.p1 - l.p0);
                    normal = glm::vec2(-n.y, n.x);
                }
            }

            if (last_collision_time < 1.0f) {
                cout << "t = " << last_collision_time << ", n = <" << normal.x << ", " << normal.y << ">" << endl;
                p.last_position = last_collision_time * (p.position - p.last_position) + p.last_position;
                p.velocity = (p.velocity - 2.f * (glm::dot(p.velocity, normal) * normal));
                p.position = p.last_position + p.velocity * dt * (1.0f - last_collision_time);
            }


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
    glm::ivec2 virtual_window_size = glm::vec2{640, 400} * 2.0f;
    glm::ivec2 actual_window_size = virtual_window_size ;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    Level level(virtual_window_size);

    Physics physics;

    auto& p1 = physics.add_particle();
    auto& p2 = physics.add_particle();
    auto& p3 = physics.add_particle();

    p1.position = glm::vec2(virtual_window_size / 2) + glm::vec2(-50, -50);
    p2.position = glm::vec2(virtual_window_size / 2) + glm::vec2(50, 50);
    p3.position = glm::vec2(virtual_window_size / 2) + glm::vec2(75, 100);

    p1.velocity.y -= 10;
    //p2.velocity.y -= 50;

    p1.set_mass(.1);
    p2.set_mass(.1);
    p3.set_mass(.1);

    auto& spring1 = physics.join_particles(p1, p2);

    spring1.spring.k = 100;
    //spring1.spring.b = 0.01;

    auto& spring2 = physics.join_particles(p2, p3);

    spring2.spring.k = 100;
    //spring2.spring.b = 0.01;

    auto& spring3 = physics.join_particles(p3, p1);

    spring2.spring.k = 100;
    //spring2.spring.b = 0.01;

    {
        auto& floor = physics.add_line();
        floor.p0 = {0, 0};
        floor.p1 = {virtual_window_size.x, 50.5};
    }

    {
        auto& floor = physics.add_line();
        floor.p0 = {0, 50.5};
        floor.p1 = {virtual_window_size.x, 0};
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

            /* do rendering */
            render();
            /* end */
        }
    );

    pixel::app().run();

}


int main(int argc, char* argv[])
{
   start(argc, argv);

   return 0;
}
