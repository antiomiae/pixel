#include <pixel/pixel.h>
#include <pixel/physics/physics.h>
#include <unistd.h>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/color_space.hpp>

using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::physics;
using namespace pixel::input;

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


class ParticleController
{
public:
    ParticleController(glm::vec4 field_size)
        : field_size_(field_size)
    {
        for (auto i = 0u; i < 50; ++i) {
            particles_.push_back(random_particle());
            particles_.back().color = random_color();
            particles_.back().init_last_position(DT);
        }
    }

    VelocityVerletParticle random_particle()
    {
        return VelocityVerletParticle::random_particle(
            field_size_,
            glm::vec2{10.f, 1000.f},
            glm::vec2{1.f, 1.f}
        );
    };

    glm::vec4 random_color()
    {
        static default_random_engine generator;
        static std::uniform_real_distribution<float> h_rand(0.f, 360.f);

        return hsv_color(h_rand(generator), 0.8f, 0.8f);
    };

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
                IntegrationMethods::velocity_verlet_step(p, acc_func, DT);
            }

        }
    }

private:
    glm::vec4 field_size_;

    vector<VelocityVerletParticle> particles_;
};


int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
        argv = &argv[2];
        argc -= 2;
    }

    glm::ivec2 virtual_window_size = glm::ivec2{320, 224};
    glm::ivec2 actual_window_size = virtual_window_size * 3;

    pixel::init(actual_window_size, virtual_window_size);
    glEnable(GL_BLEND);

    OffscreenRenderTarget render_target{};
    render_target.set_window_size(virtual_window_size);

    Level main_level{virtual_window_size};

    main_level.load_sprites(IMAGE_PATHS);

    Rocket rocket{&main_level};

    rocket.set_position(virtual_window_size / 2);

    SpriteBatch sprite_batch;

    ParticleController particles{glm::vec4{0, 0, virtual_window_size}};

    app().render_context().default_clear_color = {0.8, 0.8, 0.8, 1.0};

    pixel::app().set_tick_callback(
        [&] {
            glClear(GL_COLOR_BUFFER_BIT);
            render_target.activate();
            glClear(GL_COLOR_BUFFER_BIT);

            sprite_batch.restart();

            rocket.render(sprite_batch);

            main_level.renderer_group().get<renderers::SpriteRenderer>().render(
                sprite_batch.sprites(), main_level.sprite_texture(), main_level.camera());

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
