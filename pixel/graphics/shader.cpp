//
//

#include "shader.h"
#include "../util/util.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>


namespace pixel::graphics {

using namespace std;


void loadShader(GLuint shader, const char *path) {
    ifstream file(path);
    string text, line;

    if (!file.is_open()) {
        throw "Unable to open shader source file";
    };

    while (getline(file, line)) {
        text += line;
        text += "\n";
    };

    auto length = static_cast<GLint>(text.length());
    auto c_str = text.c_str();

    glShaderSource(shader, 1, &c_str, &length);
}

bool compileShader(GLuint shader) {
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return success == GL_TRUE;
}

bool linkProgram(GLuint program) {
    glLinkProgram(program);
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    return success == GL_TRUE;
}

void logShaderError(GLuint shader) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    vector<GLchar> log(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]);

    string text(&log[0]);

    cout << text;
}

void logProgramError(GLuint program) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    vector<GLchar> log(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &log[0]);

    string text(&log[0]);

    cout << text;
}


int programParameter(GLuint program, GLenum attribute) {
    int value;
    glGetProgramiv(program, attribute, &value);
    return value;
}


AttributeMap enumerateProgramAttributes(GLuint program) {
    AttributeMap map;

    vector<GLchar> buffer((unsigned) programParameter(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH));
    const int attr_count = programParameter(program, GL_ACTIVE_ATTRIBUTES);

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

        buffer[length+1] = '\0';
        string name(buffer.data());

        memset(attr.name, 0, 30);
        memcpy(&attr.name[0], buffer.data(), min(29, length));

        map[name] = attr;
    }

    return map;
}


AttributeMap enumerateProgramUniforms(GLuint program) {
    AttributeMap map;

    vector<GLchar> buffer((unsigned) programParameter(program, GL_ACTIVE_UNIFORM_MAX_LENGTH));
    const int uniform_count = programParameter(program, GL_ACTIVE_UNIFORMS);

    for (int i = 0; i < uniform_count; ++i) {
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

        buffer[length+1] = '\0';
        string name(buffer.data());

        memset(attr.name, 0, 30);
        memcpy(&attr.name[0], buffer.data(), min(29, length));

        map[name] = attr;
    }

    return map;
}


Shader::Shader() {

}


Shader::Shader(const char *vsPath, const char *fsPath) {
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(vs, vsPath);
    loadShader(fs, fsPath);

    if (!compileShader(vs)) {
        logShaderError(vs);
        throw "Failed to compile vertex shader";
    };

    if (!compileShader(fs)) {
        logShaderError(fs);
        throw "Failed to compile fragment shader";
    };

    auto program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    if (!linkProgram(program)) {
        logProgramError(program);
        throw "Failed to link program";
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    _programId = program;
    _attributeMap = enumerateProgramAttributes(_programId);
    _uniformMap = enumerateProgramUniforms(_programId);
}


void Shader::activate() {
    glUseProgram(_programId);
}


void Shader::deactivate() {
    glUseProgram(0);
}

Attribute Shader::attribute(const string &name) const {
    return _attributeMap.at(name);
}

Attribute Shader::uniform(const string &name) const {
    return _uniformMap.at(name);
}

void Shader::setUniform(const std::string &name, int v0) {
    glUniform1i(uniform(name).index, v0);
}

void Shader::setUniform(const std::string &name, int v0, int v1) {
    glUniform2i(uniform(name).index, v0, v1);
}

void Shader::setUniform(const std::string &name, int v0, int v1, int v2) {
    glUniform3i(uniform(name).index, v0,  v1, v2);
}

void Shader::setUniform(const std::string &name, float) {

}

void Shader::setUniform(const std::string &name, float, float) {

}

void Shader::setUniform(const std::string &name, float, float, float) {

}

};
