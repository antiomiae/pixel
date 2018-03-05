#include <pixel/graphics/camera.h>
#include <test/setup.h>

namespace
{

using namespace pixel;
using namespace pixel::graphics;

TEST(Camera, Constructor)
{
    Camera camera{
            glm::ivec2(100, 100), // window size
            glm::vec4(0, 0, 200, 200) // bounds
    };


    EXPECT_EQ(camera.position(), glm::vec2(0, 0));
    EXPECT_EQ(camera.window_size(), glm::ivec2(100, 100));
}

TEST(Camera, view_matrix)
{
    Camera camera{
            glm::ivec2(100, 100), // window size
            glm::vec4(0, 0, 200, 200) // bounds
    };

    auto v = camera.view_matrix();

    EXPECT_EQ(v, glm::mat4());

    camera.translate(glm::vec2(50.0f, 50.0f));

    EXPECT_EQ(camera.view_matrix(), glm::translate(glm::mat4(), glm::vec3(-50.0, -50.0, 0.0)));

    camera.position_at(0, 0);
    camera.scale(2.0);

    v = camera.view_matrix();

    EXPECT_EQ((v * glm::vec4(50, 50, 0, 1.0)), glm::vec4(50, 50, 0, 1.0));

    camera.translate(50, 50);

    v = camera.view_matrix();

    EXPECT_EQ((v * glm::vec4(100, 100, 0, 1.0)), glm::vec4(50, 50, 0, 1.0));
}


TEST(Camera, view_rect)
{
    Camera camera{
            glm::ivec2(100, 100), // window size
            glm::vec4(0, 0, 200, 200) // bounds
    };

    EXPECT_EQ(
            camera.view_rect(),
            glm::vec4(0, 0, 100, 100)
    );

    camera.translate(50, 50);
    EXPECT_EQ(
            camera.view_rect(),
            glm::vec4(50, 50, 150, 150)
    );

    camera.scale(2.0);
    EXPECT_EQ(
            camera.view_rect(),
            glm::vec4(75, 75, 125, 125)
    );
}

};
