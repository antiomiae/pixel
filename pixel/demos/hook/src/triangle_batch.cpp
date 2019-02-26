#include "triangle_batch.h"

using namespace pixel;
using namespace pixel::graphics;

void TriangleBatch::add_sprite(const pixel::graphics::Sprite& sprite)
{

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

}

vector<Triangle>& TriangleBatch::triangles()
{
    return tris_;
}
