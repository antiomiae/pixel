#include "pixel/pixel.h"

struct vertex {
    GLfloat position[3];
    GLfloat texture_coord[2];
};

int main() {
    pixel::App app;

    app.init();

    {
        int w, h;
        glfwGetWindowSize(app._window, &w, &h);
        glViewport(0, 0, w, h);
    }

    pixel::graphics::Texture t1(GL_TEXTURE_2D);

    {
        pixel::graphics::ImageData ground_tile = pixel::graphics::load_png("assets/ground1.png");
        t1.load(ground_tile.width, ground_tile.height, ground_tile.data);
    }

    pixel::graphics::Shader shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");
    pixel::graphics::Vao vao;
    pixel::graphics::Buffer buffer(GL_STREAM_DRAW);

    vertex vertices[] = {
            {-1, -1, 0, 0, 0},
            {1,  -1, 0, 1, 0},
            {-1, 1,  0, 0, 1},
            {1,  -1, 0, 1, 0},
            {-1, 1,  0, 0, 1},
            {1,  1,  0, 1, 1}
    };

    buffer.loadData(vertices, 6 * sizeof(vertex));

    vao.activate();
    buffer.bindToProgramAttribute(shader, "vertex_position", sizeof(vertex));
    buffer.bindToProgramAttribute(shader, "vertex_texture_coord", sizeof(vertex), offsetof(vertex, texture_coord));
    vao.deactivate();

    app.setTickCallback([&] {
        shader.activate();
        vao.activate();

        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, t1._textureId);

        shader.setUniform("tex", 1);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader.deactivate();
        vao.deactivate();

        logGlErrors();
    });

    app.run();

    return 0;
}
