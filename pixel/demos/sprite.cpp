#include "../pixel/pixel.h"
#include <unistd.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"

using pixel::graphics::Buffer;
using pixel::graphics::IndexBuffer;
using pixel::graphics::Sprite;
using pixel::graphics::TextureRegion;
using pixel::graphics::SpriteRenderer;


void updateSprite(vector<Sprite>& sprites, GLFWwindow& window_)
{
    auto window = &window_;
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

    for (auto i = 0u; i < sprites.size(); ++i) {
        sprites[i].angle += angle;
    }

    if (v[0] != 0 || v[1] != 0) {
        float inv_length = 1 / std::sqrt(v[0] * v[0] + v[1] * v[1]);

        for (auto i = 0u; i < sprites.size(); ++i) {
            sprites[i].x += v[0] * inv_length * 5;
            sprites[i].y += v[1] * inv_length * 5;
        }
    }
}


int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::cout << "Changing to directory " << argv[1] << std::endl;
        chdir(argv[1]);
    }

    pixel::print_version_information();

    pixel::App app;

    app.init();

    //if (GLEW_ARB_texture_storage) {
    std::cout << "Should have glTexStorage3D: " << &glTexStorage3D << std::endl;
    //}

    pixel::graphics::Texture t1(GL_TEXTURE_2D);

    pixel::graphics::ImageData ground_tile = pixel::graphics::load_png("assets/sonic.png");
    t1.load(ground_tile.width, ground_tile.height, ground_tile.data);

    SpriteRenderer renderer{};
    {
        auto window_size = app.render_context().window_size;

        renderer.program().activate();
        renderer.program().setUniform("projection", glm::ortho(0.0f, (float) window_size.x, 0.0f, (float) window_size.y));
        renderer.program().setUniform("tex", 0);
        logGlErrors();
        renderer.program().deactivate();
    }

    Sprite sp{};

    sp.texture_region.w = 28;
    sp.texture_region.h = 39;

    vector<Sprite> sprites = {sp, sp, sp, sp};

    for (int i = 0; i < 4; ++i) {
        sprites[i].x = i * 50 + 50;
        sprites[i].y = 50;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t1.texture_id());

    app.set_tick_callback(
        [&] {
            updateSprite(sprites, app.window());
            renderer.render(sprites, t1);
        }
    );

    app.run();

    return 0;
}


#pragma clang diagnostic pop
