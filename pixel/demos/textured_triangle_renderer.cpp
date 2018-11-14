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
    explicit SpriteRenderer2(Level& level) :
    level_{level}
    {
        tris_.reserve(200);
    }

    void add_sprite(const Sprite& sprite)
    {

        auto texture_region = sprite.texture_region;
        glm::vec2 size = {texture_region.w, texture_region.h};
        auto center = sprite.position + size * sprite.center;
        // fill corners with w, h centered quad, rotate by angle
        // add center to each corner, build triangles
        array<glm::vec2, 4> corners = {
            glm::vec2{-size.x/2, -size.y/2}, // BL
            glm::vec2{ size.x/2, -size.y/2}, // BR
            glm::vec2{-size.x/2,  size.y/2}, // TL
            glm::vec2{ size.x/2,  size.y/2}  // TR
        };

        auto rotate_matrix = glm::rotate(glm::mat3(), sprite.angle);

        std::transform(begin(corners), end(corners), begin(corners), [&](auto v) { return glm::vec2(rotate_matrix * glm::vec3(v, 1)); });

    }

private:
    vector<renderers::TexturedTriangleRenderer::Triangle> tris_;
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
        }
    );

    pixel::app().run();
}

int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}
