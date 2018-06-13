#include <sstream>
#include <fstream>
#include <iostream>

#include "shader_utils.h"
#include "graphics.h"
#include "../util/symbol_map.h"
#include "../util/util.h"
#include "../util/collections.h"
#include <pixel/error.h>


namespace pixel::graphics
{

using namespace pixel;
using namespace std;


bool compile_shader(GLuint shader)
{
    glCompileShader(shader);
    log_gl_errors();

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    log_gl_errors();

    return success == GL_TRUE;
}


void log_shader_error(GLuint shader)
{
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    log_gl_errors();

    if (maxLength == 0) {
        return;
    }

    vector<GLchar> log((unsigned) maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]);
    log_gl_errors();

    string text(&log[0]);

    cout << text;
}


void log_program_error(GLuint program)
{
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    log_gl_errors();

    vector<GLchar> log(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &log[0]);
    log_gl_errors();

    string text(&log[0]);

    cout << text;
}


bool link_program(GLuint program)
{
    glLinkProgram(program);
    log_gl_errors();

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    log_gl_errors();

    return success == GL_TRUE;
}


void load_shader(GLuint shader, const char* path)
{
    ifstream file(path);
    string text, line;

    if (!file.is_open()) {
        pixel_error("Unable to open shader source file: " + string(path));
    };

    while (getline(file, line)) {
        text += line;
        text += "\n";
    };

    auto length = static_cast<GLint>(text.length());
    auto c_str = text.c_str();

    glShaderSource(shader, 1, &c_str, &length);
    log_gl_errors();
    log_shader_error(shader);
}


int program_parameter(GLuint program, GLenum attribute)
{
    int value;
    glGetProgramiv(program, attribute, &value);
    log_gl_errors();
    return value;
}


AttributeMap enumerate_program_attributes(GLuint program)
{
    AttributeMap map;

    vector<GLchar> buffer((unsigned) program_parameter(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH));
    log_gl_errors();

    const int attr_count = program_parameter(program, GL_ACTIVE_ATTRIBUTES);
    log_gl_errors();

    for (int i = 0; i < attr_count; ++i) {
        Attribute attr;
        int length;

        attr.index = i;

        glGetActiveAttrib(
            program,
            i,
            buffer.size(),
            &length,
            &attr.size,
            &attr.type,
            &buffer[0]
        );
        log_gl_errors();

        buffer[length + 1] = '\0';

        attr.name = buffer.data();

        attr.location = glGetAttribLocation(program, attr.name.c_str());
        log_gl_errors();

        map[attr.name] = attr;
    }

    return map;
}


AttributeMap enumerate_program_uniforms(GLuint program)
{
    std::unordered_map<std::string, Attribute> attributeMap;

    vector<GLchar> buffer((unsigned) program_parameter(program, GL_ACTIVE_UNIFORM_MAX_LENGTH));
    log_gl_errors();

    const int uniform_count = program_parameter(program, GL_ACTIVE_UNIFORMS);
    log_gl_errors();

    for (auto i = 0u; i < uniform_count; ++i) {
        Attribute attr;
        int length;

        attr.index = i;

        glGetActiveUniform(
            program,
            i,
            buffer.size(),
            &length,
            &attr.size,
            &attr.type,
            &buffer[0]
        );
        log_gl_errors();

        buffer[length + 1] = '\0';

        attr.name = buffer.data();

        attr.location = glGetUniformLocation(program, attr.name.c_str());
        log_gl_errors();

        attributeMap[attr.name] = attr;
    }

    return attributeMap;
}

};
