//
//

#include "shadercheck.h"
#include "pixel/pixel.h"

#include <iostream>

using namespace std;
using pixel::App;
using pixel::graphics::Shader;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "ERROR: vertex and fragment shader paths required" << endl;
        cerr << "Usage: shadercheck VS_PATH FS_PATH" << endl;
        exit(1);
    }

    App app;
    app.init();

    glfwHideWindow(app.window_);

    Shader shader(argv[1], argv[2], "shadercheck shader under test");

    cout << shader.debugPrint() << endl;

    return 0;
}
