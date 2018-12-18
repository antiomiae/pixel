//
//

#include "shadercheck.h"
#include "pixel/pixel.h"

#include <iostream>

using namespace std;
using pixel::App;
using pixel::graphics::Shader;

int main(int argc, char *argv[]) {
    pixel::init();

    if (argc < 3) {
        cerr << "ERROR: vertex and fragment shader paths required" << endl;
        cerr << "Usage: shadercheck VS_PATH FS_PATH" << endl;
        exit(1);
    }

    glfwHideWindow(pixel::app().window());

    cout << "vs path: " << argv[1] << endl;
    cout << "fs path: " << argv[2] << endl;

    Shader shader(argv[1], argv[2]);

    cout << shader.debugPrint() << endl;

    return 0;
}
