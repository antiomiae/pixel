#include <pixel/pixel.h>
#include <iostream>

using namespace pixel;
using namespace std;

int main(int argc, char* argv[])
{
    glm::ivec2 virtual_window_size = glm::vec2{1440, 900};
    glm::ivec2 actual_window_size = virtual_window_size;

    pixel::init(actual_window_size, virtual_window_size, argc, argv);

    pixel::graphics::Shader n_body_shader{
        "assets/shaders/n_body.vert",
        "assets/shaders/n_body.frag",
        {"_position__last_position", "_vec__acc", "_mass__inv_mass"}
    };

    cout << n_body_shader.debugPrint() << endl;

    return 0;
}


