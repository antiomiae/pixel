#include <pixel/graphics/graphics.h>
#include <test/setup.h>
#include <glm/gtx/string_cast.hpp>

namespace
{

using namespace pixel;
using namespace pixel::graphics;

TEST_CASE("Camera")
{
    Camera camera{
        glm::ivec2(100, 100), // window size
        glm::vec4(0, 0, 200, 200) // bounds
    };

    SECTION("position") {
        REQUIRE(camera.position() == glm::vec2(0, 0));
    }

    SECTION("window_size") {
        REQUIRE(camera.window_size() == glm::ivec2(100, 100));
    }


    SECTION("view_matrix") {
        auto v = camera.view_matrix();

        REQUIRE(v == glm::mat4(1.0f));

        camera.translate(glm::vec2(50.0f, 50.0f));

        REQUIRE(camera.view_matrix() == glm::translate(glm::mat4(1), glm::vec3(-50.0, -50.0, 0.0)));
    }

    SECTION("view_rect") {

        REQUIRE(glm::to_string(camera.view_rect()) == glm::to_string(glm::vec4(0, 0, 100, 100)));

        camera.translate(50, 50);

        REQUIRE(camera.view_rect() == glm::vec4(50, 50, 150, 150));

        camera.scale(2.0);

        auto vr = camera.view_rect();

        REQUIRE(vr == glm::vec4(75, 75, 125, 125));
    }


    SECTION("scroll_lock") {

        camera.translate(50, 50);

        REQUIRE(camera.position() == glm::vec2(50, 50)
        );

        camera.lock_x();

        camera.translate(50, 50);

        REQUIRE(camera.position() == glm::vec2(50, 100));

        camera.lock_y();

        camera.translate(50, 50);

        REQUIRE(camera.position() == glm::vec2(50, 100));

        camera.lock_x(false);
        camera.lock_y(false);

        camera.translate(50, 50);

        REQUIRE(camera.position() == glm::vec2(100, 150));
    }
}

};
