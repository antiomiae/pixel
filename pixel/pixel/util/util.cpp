//
//
#include <fstream>
#include <iostream>
#include <string>
#include <pixel/graphics/common.h>
#include <pixel/error.h>
#include "util.h"

using namespace std;

void _log_gl_errors(const int line, const char *file) {
    string prefix = string(file) + ":" + std::to_string(line) + ":";
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        cout << prefix;
        switch (err) {
            case GL_NO_ERROR:
                cout << "GL_NO_ERROR" << endl;
                break;
            case GL_INVALID_ENUM:
                cout << "GL_INVALID_ENUM" << endl;
                break;
            case GL_INVALID_VALUE:
                cout << "GL_INVALID_VALUE" << endl;
                break;
            case GL_INVALID_OPERATION:
                cout << "GL_INVALID_OPERATION" << endl;
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl;
                break;
            case GL_OUT_OF_MEMORY:
                cout << "GL_OUT_OF_MEMORY" << endl;
                break;
        }
    }
}

namespace pixel::util
{

bool file_exists(const std::string &path)
{
    ifstream file(path);
    return file.good();
}

void _error(const int line, const char *file, const std::string &msg)
{
    string prefix = string(file) + ":" + std::to_string(line) + ":";
    cout << prefix << msg << endl;
    throw BaseError(msg);
}

void _error_if(const int line, const char *file, bool expr, const std::string &msg)
{
   if (expr) {
       _error(line, file, msg);
   }
}

};
