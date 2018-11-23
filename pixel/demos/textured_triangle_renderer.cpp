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


class SpriteRenderer2 : public TexturedTriangleRenderer
{
public:
    using Triangle = TexturedTriangleRenderer::Triangle;
    using Vertex = TexturedTriangleRenderer::Vertex;


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
        auto size = glm::vec2(texture_region.w, texture_region.h);
        auto half_size = glm::vec2(texture_region.w, texture_region.h) / 2.0f;
        auto center = glm::vec3(sprite.position + size * (-0.5f - sprite.center), 0);
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

        auto layer = sprite.texture_region.layer;

        array<glm::vec3, 4> texture_corners = {
            glm::vec3{0, 0, layer},
            glm::vec3{1, 0, layer},
            glm::vec3{0, 1, layer},
            glm::vec3{1, 1, layer}
        };

        std::for_each(
            begin(texture_corners), end(texture_corners), [&](auto& v) {
                if (sprite.flip_flags & Sprite::kHorizontal) {
                    v.x = 1.0f - v.x;
                }
                if (sprite.flip_flags & Sprite::kVertical) {
                    v.y = 1.0f - v.y;
                }
                if (sprite.flip_flags & Sprite::kDiagonal) {
                    auto t = v;
                    v.x = t.y * size.y;
                    v.y = t.x * size.x;
                }

                v *= glm::vec3(size, 1.0f);

                if (sprite.texture_region.flipped) {
                    auto t = v;
                    v.x = t.y;
                    v.y = t.x;
                }
            }
        );

        tris_.push_back(
            Triangle(
                Vertex(corners[0], texture_corners[0], sprite.color),
                Vertex(corners[1], texture_corners[1], sprite.color),
                Vertex(corners[2], texture_corners[2], sprite.color)
            )
        );

        tris_.push_back(
            Triangle(
                Vertex(corners[2], texture_corners[2], sprite.color),
                Vertex(corners[1], texture_corners[1], sprite.color),
                Vertex(corners[3], texture_corners[3], sprite.color)
            )
        );
    }

    void render(Texture& texture_atlas, Camera& camera) {
        TexturedTriangleRenderer::render(tris_, texture_atlas, camera);
    }

private:
    vector<Triangle> tris_;
    Level& level_;
};


void start(int argc, char** argv)
{
    glm::ivec2 virtual_window_size = glm::vec2{320, 240};
    glm::ivec2 actual_window_size = virtual_window_size * 2;

    pixel::init(actual_window_size, virtual_window_size, &argc, argv);

    pixel::app().set_background_color({1, 1, 1, 1});

    Level level{virtual_window_size};

    level.load_sprites({"assets/rocket.png"});

    SpriteBatch batch;

    SpriteRenderer2 sp2{level};

    auto rocket = level.get_sprite("assets/rocket.png");
    rocket.position = {160, 120};

    sp2.add_sprite(rocket);
    batch.add(rocket);

    pixel::app().set_tick_callback(
        [&] {
            level.begin_render();
            sp2.render(level.sprite_texture(), level.camera());
            //level.renderer_group().get<SpriteRenderer>().render(batch.sprites(), level.sprite_texture(), level.camera());
            level.finish_render();
        }
    );

    pixel::app().run();
}

int main(int argc, char* argv[])
{
    start(argc, argv);

    return 0;
}
