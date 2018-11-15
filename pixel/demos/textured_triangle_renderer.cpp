#include <pixel/pixel.h>
#include <unistd.h>
#include <pixel/renderers/textured_triangle_renderer.h>
#include <pixel/level.h>
#include <vector>
#include <array>


using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;
using namespace pixel::renderers;


class SpriteRenderer2
{
public:
    using Triangle = TexturedTriangleRenderer::Triangle;

    explicit SpriteRenderer2(Level& level)
        :
        level_{level}
    {
        tris_.reserve(200);
    }

    void add_sprite(const Sprite& sprite)
    {
        if (tris_.size() == tris_.capacity()) {
            return;
        }

        auto texture_region = sprite.texture_region;
        auto half_size = glm::vec2(texture_region.w, texture_region.h) / 2.0f;
        auto center = glm::vec3(sprite.position + half_size * sprite.center, 0);
        // fill corners with w, h centered quad, rotate by angle
        // add center to each corner, build triangles
        array<glm::vec3, 4> corners = {
            glm::vec3{-half_size.x, -half_size.y, 0}, // BL
            glm::vec3{half_size.x, -half_size.y, 0}, // BR
            glm::vec3{-half_size.x, half_size.y, 0}, // TL
            glm::vec3{half_size.x, half_size.y, 0}  // TR
        };

        auto rotate_matrix = glm::rotate(glm::mat3(), sprite.angle);

        std::transform(
            begin(corners), end(corners), begin(corners),
            [&](auto v) { return rotate_matrix * v + center; }
        );

        tris_.push_back(Triangle(corners[0], corners[1], corners[2]));
        tris_.push_back(Triangle(corners[1], corners[2], corners[3]));
    }

private:
    vector<Triangle> tris_;
    Level& level_;
};


void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{320, 240};
    glm::ivec2 actual_window_size = virtual_window_size * 3;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    pixel::app().set_background_color({1, 1, 1, 1});

    Level level{virtual_window_size};

    level.load_sprites({"assets/guy.png", "assets/rocket.png"});

    SpriteRenderer2 sp2{level};

    pixel::app().set_tick_callback(
        [&] {
            sp2.add_sprite(level.get_sprite("assets/rocket.png"));
        }
    );

    pixel::app().run();
}

int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}
