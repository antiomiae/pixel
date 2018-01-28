//
//

#include "shader.h"
#include "../util/symbol_map.h"
#include "../util/util.h"
#include "../util/collections.h"
#include <iostream>
#include <sstream>
#include <fstream>


namespace pixel::graphics
{

using namespace std;

using pixel::collections::entries;

string Attribute::debugPrint() const
{
    pixel::util::SymbolMap symbols = pixel::util::symbolMap();

    stringstream out;

    out << "Attribute(" << endl;
    out << "  name = " << this->name << endl;
    out << "  location = " << this->location << endl;
    out << "  index = " << this->index << endl;
    out << "  size = " << this->size << endl;
    out << "  type = " << symbols.at(this->type)[0] << endl;
    out << ")" << endl;

    return out.str();
}


void loadShader(GLuint shader, const char *path)
{
    ifstream file(path);
    string text, line;

    if (!file.is_open()) {
        error("Unable to open shader source file");
    };

    while (getline(file, line)) {
        text += line;
        text += "\n";
    };

    auto length = static_cast<GLint>(text.length());
    auto c_str = text.c_str();

    glShaderSource(shader, 1, &c_str, &length);
}

bool compileShader(GLuint shader)
{
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return success == GL_TRUE;
}

bool linkProgram(GLuint program)
{
    glLinkProgram(program);
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    return success == GL_TRUE;
}

void logShaderError(GLuint shader)
{
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    vector<GLchar> log(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]);

    string text(&log[0]);

    cout << text;
}

void logProgramError(GLuint program)
{
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    vector<GLchar> log(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &log[0]);

    string text(&log[0]);

    cout << text;
}


int programParameter(GLuint program, GLenum attribute)
{
    int value;
    glGetProgramiv(program, attribute, &value);
    return value;
}


AttributeMap enumerateProgramAttributes(GLuint program)
{
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

        buffer[length + 1] = '\0';

        attr.name = buffer.data();

        attr.location = glGetAttribLocation(program, attr.name.c_str());

        map[attr.name] = attr;
    }

    return map;
}


AttributeMap enumerateProgramUniforms(GLuint program)
{
    std::unordered_map<std::string, Attribute> attributeMap;

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

        buffer[length + 1] = '\0';

        attr.name = buffer.data();

        attr.location = glGetUniformLocation(program, attr.name.c_str());

        attributeMap[attr.name] = attr;
    }

    int count = attributeMap.size();

    return attributeMap;
}


Shader::Shader()
{
    cout << "Default constructor called" << endl;
}


Shader::Shader(const char *vsPath, const char *fsPath, const char *debugName)
        : _debugName(debugName)
{
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(vs, vsPath);
    loadShader(fs, fsPath);

    if (!compileShader(vs)) {
        logShaderError(vs);
        error("Failed to compile vertex shader");
    };

    if (!compileShader(fs)) {
        logShaderError(fs);
        error("Failed to compile fragment shader");
    };

    auto program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    if (!linkProgram(program)) {
        logProgramError(program);
        error("Failed to link program");
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    _programId = program;
    _attributeMap = enumerateProgramAttributes(_programId);
    _uniformMap = enumerateProgramUniforms(_programId);
}


void Shader::activate()
{
    glUseProgram(_programId);
}


void Shader::deactivate()
{
    glUseProgram(0);
}

Attribute Shader::attribute(const string &name) const
{
    try {
        auto attr = _attributeMap.at(name);
        return attr;
    } catch (exception& ex) {
        error("Unable to find attribute named `" + name + "`");
        throw ex;
    }

}

Attribute Shader::uniform(const string &name) const
{
    try {
        auto attr = _uniformMap.at(name);
        return attr;
    } catch (exception& ex) {
        error("Unable to find uniform named `" + name + "`");
        throw ex;
    }
}

void Shader::setUniform(const std::string &name, int v0)
{
    glProgramUniform1i(_programId, uniform(name).location, v0);
    //glUniform1i(uniform(name).location, v0);
}

void Shader::setUniformArray(const std::string &name, int count, const int *arr)
{
    glUniform1iv(uniform(name).location, count, arr);
}

void Shader::setUniform(const std::string &name, int v0, int v1)
{
    glUniform2i(uniform(name).location, v0, v1);
}

void Shader::setUniform(const std::string &name, int v0, int v1, int v2)
{
    glUniform3i(uniform(name).location, v0, v1, v2);
}

void Shader::setUniform(const std::string &name, float v0)
{
    glUniform1f(uniform(name).location, v0);
}

void Shader::setUniform(const std::string &name, float v0, float v1)
{
    glUniform2f(uniform(name).location, v0, v1);
}

void Shader::setUniform(const std::string &name, const glm::vec2 &value)
{
    glUniform2fv(uniform(name).location, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &name, float v0, float v1, float v2)
{
    glUniform3f(uniform(name).location, v0, v1, v2);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &value)
{
    glUniform3fv(uniform(name).location, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &name, float v0, float v1, float v2, float v3)
{
    glUniform4f(uniform(name).location, v0, v1, v2, v3);
}

void Shader::setUniform(const std::string &name, const glm::vec4 &value)
{
    glUniform4fv(uniform(name).location, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &name, const glm::mat2 &value)
{
    glUniformMatrix2fv(uniform(name).location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &name, const glm::mat3 &value)
{
    glUniformMatrix3fv(uniform(name).location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &name, const glm::mat4 &value)
{
    float mat[16];
    std::memcpy(&mat, glm::value_ptr(value), sizeof(mat));
    glUniformMatrix4fv(uniform(name).location, 1, GL_FALSE, mat);
}

void Shader::setUniformArray(const std::string &name, const int count, const float arr[])
{
    glUniform1fv(uniform(name).location, count, arr);
}

void Shader::setUniformArray(const std::string &name, const int count, const glm::vec2 arr[])
{
    glUniform2fv(uniform(name).location, count, glm::value_ptr(arr[0]));
}

void Shader::setUniformArray(const std::string &name, const int count, const glm::vec3 arr[])
{
    glUniform3fv(uniform(name).location, count, glm::value_ptr(arr[0]));
}

void Shader::setUniformArray(const std::string &name, const int count, const glm::vec4 arr[])
{
    glUniform4fv(uniform(name).location, count, glm::value_ptr(arr[0]));
}

void Shader::setUniformArray(const std::string &name, const int count, const glm::mat2 arr[])
{
    glUniformMatrix2fv(uniform(name).location, count, GL_FALSE, glm::value_ptr(arr[0]));
}

void Shader::setUniformArray(const std::string &name, const int count, const glm::mat3 arr[])
{
    glUniformMatrix3fv(uniform(name).location, count, GL_FALSE, glm::value_ptr(arr[0]));
}

void Shader::setUniformArray(const std::string &name, const int count, const glm::mat4 arr[])
{
    glUniformMatrix4fv(uniform(name).location, count, GL_FALSE, glm::value_ptr(arr[0]));
}

std::string Shader::debugPrint() const
{
    auto uniforms = entries(_uniformMap);
    std::vector<Attribute> attributes = entries(_attributeMap);

    std::sort(begin(uniforms), end(uniforms), [](auto a, auto b) { return a.location < b.location; });
    std::sort(begin(attributes), end(attributes), [](auto a, auto b) { return a.index < b.index; });

    stringstream out;

    out << "** Shader uniforms **" << endl;
    std::for_each(begin(uniforms), end(uniforms), [&](auto &attr) { out << attr.debugPrint() << endl; });

    out << "** Shader attributes **" << endl;
    std::for_each(begin(attributes), end(attributes), [&](auto &attr) { out << attr.debugPrint() << endl; });

    return out.str();
}

};
