#include <pixel/pixel.h>
#include <pixel/physics/physics.h>
#include <unistd.h>
#include <algorithm>
#include <glm/gtx/component_wise.hpp>

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
        for_each(begin(paths), end(paths), [&](auto &path) { texture_atlas_.add_image(path); });
        texture_atlas_.stop_batch();

        sprite_texture_ = texture_atlas_.as_texture();
    }

    Sprite get_sprite(string const& path)
    {
        Sprite s;
        s.texture_region = texture_atlas_.lookup(path);
        return s;
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
    }

    float rotation()
    {
        return rotation_;
    }

    float set_rotation(float r)
    {
        rotation_ = r;
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
        temp.angle += rotation_;
        temp.position = glm::vec3(position_, 0.f) + glm::rotate(glm::mat3(), temp.angle) * glm::vec3(temp.position.x, temp.position.y, 1.0f);
        batch.add(temp);

        temp = flames_;
        temp.angle += rotation_;
        temp.position = glm::vec3(position_, 0.f) + glm::rotate(glm::mat3(), temp.angle) * glm::vec3(temp.position.x, temp.position.y, 1.0f);
        batch.add(temp);
    }

private:
    Level* level_;

    float rotation_;

    glm::vec2 position_;

    VelocityVerletParticle particle_;

    Sprite body_;

    Sprite flames_;
};


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

    vector<VelocityVerletParticle> particles;


    glEnable(GL_BLEND);


    pixel::app().set_tick_callback(
        [&] {
            glClear(GL_COLOR_BUFFER_BIT);

            render_target.activate();

            //glClear(GL_COLOR_BUFFER_BIT);


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
