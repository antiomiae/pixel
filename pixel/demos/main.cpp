#include "../pixel/pixel.h"

using pixel::graphics::Buffer;

struct vertex
{
    GLfloat position[3];
    GLfloat texture_coord[2];
};

int main()
{
    pixel::App app;

    app.init();

    int w, h;
    glfwGetWindowSize(app._window, &w, &h);
    glViewport(0, 0, w, h);

    pixel::graphics::Texture t1(GL_TEXTURE_2D);

    pixel::graphics::ImageData ground_tile = pixel::graphics::load_png("assets/ground1.png");
    t1.load(ground_tile.width, ground_tile.height, ground_tile.data);

    pixel::graphics::Shader shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");
    cout << shader.debugPrint() << endl;

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

    shader.activate();

    vao.activate();

    buffer.bindToProgramAttribute(shader, "vertex_position", sizeof(vertex));

    buffer.bindToProgramAttribute(shader, "vertex_texture_coord", sizeof(vertex),
                                  static_cast<int>(offsetof(vertex, texture_coord)));

    {
        Buffer model_mat;
        glm::mat2x3 mats[6][2];
        model_mat.loadData(glm::value_ptr(mats[0][0]), sizeof(mats));
        logGlErrors();

        model_mat.bindToProgramAttribute(shader, "model_mat");
        logGlErrors();

        Buffer some_var;
        int some_vars[6];
        some_var.loadData(some_vars, sizeof(some_vars));
        logGlErrors();

        some_var.bindToProgramAttribute(shader, "some_var");
        logGlErrors();

        Buffer single_mat;
        glm::mat2 single_mats[6];
        single_mat.loadData(glm::value_ptr(single_mats[0]), sizeof(single_mats));
        logGlErrors();

        single_mat.bindToProgramAttribute(shader, "single_mat");
        logGlErrors();


        Buffer int_arr;
        int int_arrs[6][4];
        int_arr.loadData(int_arrs, sizeof(single_mats));
        logGlErrors();

        int_arr.bindToProgramAttribute(shader, "int_arr");
        logGlErrors();
    }

    shader.setUniform("projection", glm::ortho(0.0f, (float) w, 0.0f, (float) h));
    logGlErrors();

    auto view = glm::translate(glm::mat4(1.0f), {w / 2.0f, h / 2.0f, 0.0f});
    auto model = glm::scale(glm::mat4(1.0f), {100.0f, 100.0f, 1.0f});

    shader.setUniform("view", view);
    logGlErrors();
    shader.setUniform("model", model);
    logGlErrors();

    vao.deactivate();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t1._textureId);

    shader.setUniform("tex", 0);
    logGlErrors();

    app.setTickCallback([&] {
        shader.activate();
        vao.activate();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader.deactivate();
        vao.deactivate();

        logGlErrors();
    });

    app.run();

    return 0;
}
