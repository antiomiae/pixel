#include <pixel/pixel.h>
#include <pixel/physics/physics.h>
#include <unistd.h>
#include <functional>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/fast_square_root.hpp>


using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::physics;
using namespace pixel::input;

using ParticleType = VelocityVerletParticle;

const float DT = 1.0 / 60.0f;


/*
    auto max_force = 10000000.0f;
    auto force = max_force;

    auto acc_func = [&](auto& p) -> glm::vec2 {
        auto r = glm::vec2{virtual_window_size} / 2.0f - p.position;
        auto distance = glm::dot(r, r);
        r = glm::normalize(r);

        return (force * p.mass * r / distance);
    };
 */

vector<string> IMAGE_PATHS = {
    "assets/rocket.png",
    "assets/flames.png"
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


class Rocket
{
public:
    Rocket(Level* level)
    {
        body_ = level->get_sprite("assets/rocket.png");
        flames_ = level->get_sprite("assets/flames.png");

        body_.center = {0.5, 0.5};
        flames_.center = {0.5, 1.0};
        flames_.position = body_.position - glm::vec3(0.f, body_.texture_region.h / 2.f + 1.f, 0.f);
    }

    float rotation()
    {
        return angle_;
    }

    void set_rotation(float r)
    {
        angle_ = r;
    }

    glm::vec2 position()
    {
        return position_;
    }

    void set_position(glm::vec2 const& p)
    {
        position_ = p;
    }

    void render(SpriteBatch& batch)
    {
        auto temp = body_;
        temp.angle += angle_;
        temp.position = glm::vec3(position_, 0.f) +
                        glm::rotate(glm::mat3(), temp.angle) * glm::vec3(temp.position.x, temp.position.y, 1.0f);
        batch.add(temp);

        temp = flames_;
        temp.angle += angle_;
        temp.position = glm::vec3(position_, 0.f) +
                        glm::rotate(glm::mat3(), temp.angle) * glm::vec3(temp.position.x, temp.position.y, 1.0f);
        batch.add(temp);
    }

private:
    Level* level_;

    float angle_{0.f};

    glm::vec2 position_;

    VelocityVerletParticle particle_;

    Sprite body_;

    Sprite flames_;
};


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


struct Attractor
{
    glm::vec2 position{};
    float force{0.f};
};


class ParticleController
{
public:

    using StepFunction = std::function<void(vector<ParticleType>&,float,float)>;

    ParticleController(uint num_particles, glm::vec4 field_size)
        : num_particles_(num_particles),
          field_size_(field_size),
          particles_(num_particles)

    {
        reset();
    }

    void reset()
    {
        particles_.clear();

        for (auto i = 0u; i < num_particles_; ++i) {
            particles_.push_back(random_particle());
            particles_.back().color = hsv_color(
                (particles_.back().mass - 0.01f) / (100.f - 0.01f) * 360.f, 0.8f, 0.8f
            );
            particles_.back().color.a = 0.3f;
            particles_.back().init_last_position(DT);
        }
    }

    VelocityVerletParticle random_particle()
    {
        return VelocityVerletParticle::random_particle(
            field_size_,
            speed_range_,
            mass_range_
        );
    };

    glm::vec4 random_color()
    {
        auto& generator = pixel::random::default_engine();

        static std::uniform_real_distribution<float> h_rand(0.f, 360.f);

        auto c = hsv_color(h_rand(generator), 0.8f, 0.8f);
        c.a = 0.3f;
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

    uint num_particles()
    {
        return num_particles_;
    }

    void set_num_particles(uint n) {
        num_particles_ = n;
    }

    glm::vec2 speed_range()
    {
        return speed_range_;
    }

    void set_speed_range(glm::vec2 s)
    {
        speed_range_ = s;
    }

    glm::vec2 mass_range()
    {
        return mass_range_;
    }

    void set_mass_range(glm::vec2 m)
    {
        mass_range_ = m;
    }

    vector<Attractor>& attractors()
    {
        return attractors_;
    }

    glm::vec2 force_on_particle(const VelocityVerletParticle& particle)
    {
        glm::vec2 acc{};

        for (auto& attractor : attractors_) {
            auto r = attractor.position - particle.position;
            auto d = glm::fastInverseSqrt(glm::dot(r, r));
            auto force = attractor.force * powf(d, 3) * r;

            acc += force;
        }

        return acc;
    }

    template <typename AccFunc>
    void step(AccFunc& acc_func)
    {
        for (auto& p : particles_) {
            if (p.position.x < field_size_.x ||
                p.position.x > field_size_.z ||
                p.position.y < field_size_.y ||
                p.position.y > field_size_.w)
            {
                p = random_particle();
                p.color = random_color();
                p.init_last_position(DT);
            } else {
                IntegrationMethods::velocity_verlet_step(p, std::mem_fn(&ParticleController::force_on_particle), DT);
            }

        }
    }


private:
    uint num_particles_{0};
    glm::vec4 field_size_;
    glm::vec2 speed_range_{0.1f, 10.0f};
    glm::vec2 mass_range_{0.1f, 100.0f};
    vector<ParticleType> particles_;
    vector<Attractor> attractors_;
    StepFunction step_callback_;
};





int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
        argv = &argv[2];
        argc -= 2;
    }

    glm::ivec2 virtual_window_size{320, 224};
    auto actual_window_size = virtual_window_size * 4;

    pixel::init(actual_window_size, virtual_window_size);

    OffscreenRenderTarget render_target{};

    render_target.set_window_size(virtual_window_size);

    Level main_level{virtual_window_size};

    main_level.load_sprites(IMAGE_PATHS);

    Rocket rocket{&main_level};

    rocket.set_position(virtual_window_size / 2);

    SpriteBatch sprite_batch{};

    ParticleController particle_controller{100, glm::vec4{0, 0, virtual_window_size}};

    app().render_context().default_clear_color = {0.1, 0.1, 0.1, 1.0};


    particle_controller.set_speed_range({0.1f, 10.0f});
    particle_controller.reset();


    pixel::app().set_tick_callback(
        [&] {
            glClear(GL_COLOR_BUFFER_BIT);
            render_target.activate();
            glClear(GL_COLOR_BUFFER_BIT);

//            sprite_batch.restart();
//
//            rocket.render(sprite_batch);
//
//
//            main_level.renderer_group().get<renderers::SpriteRenderer>().render(
//                sprite_batch.sprites(), main_level.sprite_texture(), main_level.camera());

            render_particles(
                particle_controller.particles(),
                main_level.renderer_group().get<renderers::LineRenderer>(),
                    main_level.camera()
            );



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
