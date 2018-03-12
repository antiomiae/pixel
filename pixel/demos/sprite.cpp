#include <unistd.h>
#include <pixel/pixel.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"

using namespace pixel;
using namespace pixel::graphics;

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

    print_version_information();

    pixel::App app{
        {640, 480},
        {0.1, 0.1, 0.1, 1.0},
        1
    };

    app.init();

    TextureAtlas atlas{};

    atlas.start_batch();

    vector<string> sprite_file_names = {
        "assets/sonic.png",
        "assets/sample_sprites/1.png",
        "assets/sample_sprites/2.png",
        "assets/sample_sprites/3.png",
        "assets/sample_sprites/4.png",
        "assets/sample_sprites/5.png",
        "assets/sample_sprites/6.png"
    };

    std::for_each(
        cbegin(sprite_file_names),
        cend(sprite_file_names),
        [&](const auto& s) { atlas.add_image(s); }
    );

    atlas.stop_batch();

    auto sprite_texture_array = atlas.as_texture();
    logGlErrors();

    auto& layers = atlas.layers();
    for (auto i = 0u; i < layers.size(); ++i) {
        auto path = "debug/tex_atlas_" + to_string(i) + ".png";
        layers[i].save(path);
    }

    cout << atlas.debug_print() << endl;


    SpriteRenderer renderer{};


    auto window_size = app.render_context().window_size;


    vector<Sprite> sprites;

    auto x = 0u;

    std::for_each(
        cbegin(sprite_file_names),
        cend(sprite_file_names),
        [&](const auto& s) {
            Sprite sp{};
            sp.y = 150;
            sp.x = x + 50;
            x += 30;
            sp.texture_region = atlas.lookup(s);
            sprites.push_back(sp);
        }
    );

    sprite_texture_array.activate(0);

    Camera camera({320, 240}, {0, 0, 10000, 10000});

    app.set_tick_callback(
        [&] {
            glm::vec2 v{};
            auto window = app.window();

            //updateSprite(sprites, app.window());
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

            camera.translate(v * 10.0f);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            renderer.render(
                sprites,
                sprite_texture_array,
                camera
            );
        }
    );

    app.run();

    return 0;
}


#pragma clang diagnostic pop
