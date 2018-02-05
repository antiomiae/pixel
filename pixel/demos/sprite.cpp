#include "../pixel/pixel.h"
#include <unistd.h>
#include <cmath>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"

using pixel::graphics::Buffer;
using pixel::graphics::IndexBuffer;
using pixel::graphics::Sprite;
using pixel::graphics::TextureRegion;
using pixel::graphics::SpriteRenderer;

void updateSprite(Sprite *sprites, int length, GLFWwindow *window)
{
    float v[2] = {0};
    float angle = 0;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        v[1] = 1;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        v[1] = -1;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        v[0] = -1;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        v[0] = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_A)) {
        angle += 0.1;
    } else if (glfwGetKey(window, GLFW_KEY_F)) {
        angle -= 0.1;
    }

    for (int i = 0; i < length; ++i) {
        sprites[i].angle += angle;
    }

    if (v[0] != 0 || v[1] != 0) {
        float inv_length = 1 / std::sqrt(v[0] * v[0] + v[1] * v[1]);

        for (int i = 0; i < length; ++i) {
            sprites[i].x += v[0] * inv_length * 5;
            sprites[i].y += v[1] * inv_length * 5;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
    }

    pixel::print_version_information();

    pixel::App app;

    app.init();

    pixel::graphics::Texture t1(GL_TEXTURE_2D);

    pixel::graphics::ImageData ground_tile = pixel::graphics::load_png("assets/sonic.png");
    t1.load(ground_tile._width, ground_tile._height, ground_tile.data);

    pixel::graphics::Shader sprite_shader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
    cout << sprite_shader.debugPrint() << endl;

    {
        auto [w, h] = app.windowSize();

        sprite_shader.activate();
        sprite_shader.setUniform("projection", glm::ortho(0.0f, (float) w, 0.0f, (float) h));
        sprite_shader.setUniform("tex", 0);
        logGlErrors();
        sprite_shader.deactivate();
    }

    SpriteRenderer renderer(sprite_shader);

    Sprite sp{};

    sp.texture_region.w = 28;
    sp.texture_region.h = 39;

    Sprite sprites[4] = {sp, sp, sp, sp};

    for (int i = 0; i < 4; ++i) {
        sprites[i].x = i * 50 + 50;
        sprites[i].y = 50;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t1._textureId);

    app.setTickCallback(
            [&] {
                updateSprite(sprites, 4, app._window);
                renderer.render(sprites, 4);
            }
    );

    app.run();

    return 0;
}

#pragma clang diagnostic pop

// fa5c4b2bjk
