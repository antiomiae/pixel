#include <pixel/pixel.h>
#include <unistd.h>
#include <pixel/renderers/textured_triangle_renderer.h>
#include <pixel/level.h>
#include <vector>
#include <array>
#include <sstream>


using namespace std;
using namespace pixel;
using namespace pixel::graphics;
using namespace pixel::input;
using namespace pixel::renderers;


using Triangle = TexturedTriangleRenderer::Triangle;
using Vertex = TexturedTriangleRenderer::Vertex;

string debug_print(const glm::vec2& v, int indent = 0) {
    stringstream s;
    string pad = string(indent, ' ');
    s << pad << "vec2 {" << v.x << ", " << v.y << "}";
    return s.str();
}

string debug_print(const glm::vec3& v, int indent = 0) {
    stringstream s;
    string pad = string(indent, ' ');
    s << pad << "vec3 {" << v.x << ", " << v.y << ", " << v.z << "}";
    return s.str();
}

string debug_print(const glm::vec4& v, int indent = 0) {
    stringstream s;
    string pad = string(indent, ' ');
    s << pad << "vec4 {" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
    return s.str();
}

string debug_print(const Vertex& v, int indent = 0) {
    stringstream s;
    string pad = string(indent, ' ');
    s << "Vertex {\n"
      << pad << "  position = " << debug_print(v.position, 0) << endl
      << pad << "  color = " << debug_print(v.color, 0) << endl
      << pad << "  tex_coord = " << debug_print(v.tex_coord, 0) << endl
      << pad << "}";
    return s.str();
}

string debug_print(const Triangle& t, int indent = 0) {
    stringstream s;
    string pad = string(indent, ' ');
    s << "Triangle {\n"
      << pad << "  a = " << debug_print(t.a, indent+2) << endl
      << pad << "  b = " << debug_print(t.b, indent+2) << endl
      << pad << "  c = " << debug_print(t.c, indent+2) << endl
      << pad << "}";
    return s.str();
}


class SpriteRenderer2 : public TexturedTriangleRenderer
{
public:


    explicit SpriteRenderer2(Level& level)
        :
        level_{level}
    {
        tris_.reserve(200);
    }

    void add_sprite(const Sprite& sprite)
    {
//        if (tris_.size() == tris_.capacity()) {
//            return;
//        }

        auto texture_region = sprite.texture_region;
        auto size = glm::vec2(texture_region.w, texture_region.h);
        auto half_size = glm::vec2(texture_region.w, texture_region.h) * sprite.scale / 2.0f;
        auto center = glm::vec3(sprite.position + size * sprite.scale * (0.5f - sprite.center), 0);
        // fill corners with w, h centered quad, rotate by angle
        // add center to each corner, build triangles
        array<glm::vec3, 4> corners = {
            glm::vec3{-half_size.x, -half_size.y, 0}, // BL
            glm::vec3{half_size.x, -half_size.y, 0}, // BR
            glm::vec3{-half_size.x, half_size.y, 0}, // TL
            glm::vec3{half_size.x, half_size.y, 0}  // TR
        };

        auto rotate_matrix = glm::rotate(glm::mat3(1), sprite.angle);

        std::transform(
            begin(corners), end(corners), begin(corners),
            [&](auto v) { return rotate_matrix * v + center; }
        );

        auto layer = sprite.texture_region.layer;

        array<glm::vec3, 4> texture_corners = {
            glm::vec3{0, 1, layer},
            glm::vec3{1, 1, layer},
            glm::vec3{0, 0, layer},
            glm::vec3{1, 0, layer}
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

//        for(const auto& triangle : tris_) {
//            cout << debug_print(triangle) << endl;
//        }
    }

    void render(Texture& texture_atlas, Camera& camera)
    {
        TexturedTriangleRenderer::render(tris_, texture_atlas, camera);
    }

    void clear()
    {
        tris_.clear();
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

    pixel::app().set_background_color({0.1, 0.1, 0.1, 1});

    Level level{virtual_window_size};

    level.load_sprites({"assets/guy.png"});

    SpriteRenderer2 sp2{level};

    auto rocket = level.get_sprite("assets/guy.png");
    rocket.position = {160, 120};
    rocket.center = {0.0, 0.0};
    rocket.scale = {1, 1};

    auto count = 0u;
    for (auto i = 0u; i <= 320.0f / rocket.texture_region.w * 3; ++i) {
        for (auto j = 0u; j <= 240.0f / rocket.texture_region.h * 30; ++j) {
            auto sprite = rocket;
            sprite.position.x = i * sprite.texture_region.w/3.0f;
            sprite.position.y = j * sprite.texture_region.h/30.0f;
            sp2.add_sprite(sprite);
            count++;
        }
    }
    cout << "sprites: " << count << endl;

    pixel::app().set_tick_callback(
        [&] {


            level.begin_render();
            sp2.render(level.sprite_texture(), level.camera());
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
