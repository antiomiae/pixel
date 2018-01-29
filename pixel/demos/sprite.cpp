#include "../pixel/pixel.h"
#include <unistd.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"

using pixel::graphics::Buffer;
using pixel::graphics::IndexBuffer;
using pixel::graphics::Sprite;
using pixel::graphics::TextureRegion;

struct Vertex
{
    GLfloat position[2];
    GLfloat texture_coord[2];
};

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

    pixel::graphics::ImageData ground_tile = pixel::graphics::load_png("assets/ground1.png");
    t1.load(ground_tile.width, ground_tile.height, ground_tile.data);

    pixel::graphics::Shader sprite_shader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
    cout << sprite_shader.debugPrint() << endl;

    pixel::graphics::Vao vao;

    sprite_shader.activate();
    vao.activate();

    Buffer vertex_buffer(GL_STATIC_DRAW);
    Buffer sprite_buffer(GL_STREAM_DRAW);

    vertex_buffer.bindToProgramAttribute(
            sprite_shader,
            "vertex",
            sizeof(Vertex),
            offsetof(Vertex, position)
    );

    vertex_buffer.bindToProgramAttribute(
            sprite_shader,
            "texture_coord",
            sizeof(Vertex),
            offsetof(Vertex, texture_coord)
    );

    sprite_buffer.bindToProgramAttribute(
            sprite_shader,
            "position",
            sizeof(Sprite),
            offsetof(Sprite, position),
            1
    );

    sprite_buffer.bindToProgramAttribute(
            sprite_shader,
            "center",
            sizeof(Sprite),
            offsetof(Sprite, center),
            1
    );

    sprite_buffer.bindToProgramAttribute(
            sprite_shader,
            "angle",
            sizeof(Sprite),
            offsetof(Sprite, angle),
            1
    );

    sprite_buffer.bindToProgramAttribute(
            sprite_shader,
            "texture_region",
            sizeof(Sprite),
            offsetof(Sprite, texture_region) + offsetof(TextureRegion, rect),
            1
    );

    sprite_buffer.bindToProgramAttribute(
            sprite_shader,
            "texture_layer",
            sizeof(Sprite),
            offsetof(Sprite, texture_region) + offsetof(TextureRegion, layer),
            1
    );

    Vertex vertices[] = {
            {0, 0, 0, 1},
            {1, 0, 1, 1},
            {1, 1, 1, 0},
            {0, 1, 0, 0}
    };

    Sprite sp;

    sp.x = 200;
    sp.y = 100;

    sp.center[0] = sp.center[1] = 0.0;

    sp.texture_region.w = 16;
    sp.texture_region.h = 16;
    sp.texture_region.layer = 0;

    Sprite sprites[] = {sp, sp, sp, sp};

    vertex_buffer.loadData(vertices, sizeof(vertices));
    sprite_buffer.loadData(sprites, sizeof(sprites));

    IndexBuffer<uint8_t> index_buffer;
    logGlErrors();
    {
        uint8_t indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        index_buffer.loadData(indices, sizeof(indices));
        logGlErrors();
    }

    int w, h;
    std::tie(w, h) = app.windowSize();

    sprite_shader.setUniform("projection", glm::ortho(0.0f, (float) w, 0.0f, (float) h));
    logGlErrors();

    vao.deactivate();
    logGlErrors();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t1._textureId);

    sprite_shader.setUniform("tex", 0);
    logGlErrors();

    app.setTickCallback(
            [&] {
                sprite_shader.activate();
                logGlErrors();

                vao.activate();

                index_buffer.bind();

                logGlErrors();

                sprites[0].angle += 0.1;
                sprite_buffer.loadData(sprites, sizeof(sprites));

                //glDrawArrays(GL_TRIANGLES, 0, 6);
                glDrawElementsInstanced(GL_TRIANGLES, 6, index_buffer.elementType(), 0, 1);
                logGlErrors();
                //glDrawElements(GL_TRIANGLES, 6, index_buffer.elementType(), 0);

                sprite_shader.deactivate();
                vao.deactivate();

                logGlErrors();
            }
    );

    app.run();

    return 0;
}

#pragma clang diagnostic pop
