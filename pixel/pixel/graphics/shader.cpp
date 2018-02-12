//
//

#include "shader.h"
#include "shader_utils.h"
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


Shader::Shader()
{
    cout << "Default constructor called" << endl;
}


Shader::Shader(const char* vsPath, const char* fsPath, const char* debugName)
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


Attribute Shader::attribute(const string& name) const
{
    try {
        auto attr = _attributeMap.at(name);
        return attr;
    } catch (exception& ex) {
        error("Unable to find attribute named `" + name + "`");
        throw ex;
    }

}


Attribute Shader::uniform(const string& name) const
{
    try {
        auto attr = _uniformMap.at(name);
        return attr;
    } catch (exception& ex) {
        error("Unable to find uniform named `" + name + "`");
        throw ex;
    }
}


void Shader::setUniform(const std::string& name, int v0)
{
    glProgramUniform1i(_programId, uniform(name).location, v0);
    //glUniform1i(uniform(name).location, v0);
}


void Shader::setUniformArray(const std::string& name, int count, const int* arr)
{
    glUniform1iv(uniform(name).location, count, arr);
}


void Shader::setUniform(const std::string& name, int v0, int v1)
{
    glUniform2i(uniform(name).location, v0, v1);
}


void Shader::setUniform(const std::string& name, int v0, int v1, int v2)
{
    glUniform3i(uniform(name).location, v0, v1, v2);
}


void Shader::setUniform(const std::string& name, float v0)
{
    glUniform1f(uniform(name).location, v0);
}


void Shader::setUniform(const std::string& name, float v0, float v1)
{
    glUniform2f(uniform(name).location, v0, v1);
}


void Shader::setUniform(const std::string& name, const glm::vec2& value)
{
    glUniform2fv(uniform(name).location, 1, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(uniform(name).location, v0, v1, v2);
}


void Shader::setUniform(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(uniform(name).location, 1, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(uniform(name).location, v0, v1, v2, v3);
}


void Shader::setUniform(const std::string& name, const glm::vec4& value)
{
    glUniform4fv(uniform(name).location, 1, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, const glm::mat2& value)
{
    glUniformMatrix2fv(uniform(name).location, 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, const glm::mat3& value)
{
    glUniformMatrix3fv(uniform(name).location, 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, const glm::mat4& value)
{
    float mat[16];
    std::memcpy(&mat, glm::value_ptr(value), sizeof(mat));
    glUniformMatrix4fv(uniform(name).location, 1, GL_FALSE, mat);
}


void Shader::setUniformArray(const std::string& name, const int count, const float arr[])
{
    glUniform1fv(uniform(name).location, count, arr);
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::vec2 arr[])
{
    glUniform2fv(uniform(name).location, count, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::vec3 arr[])
{
    glUniform3fv(uniform(name).location, count, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::vec4 arr[])
{
    glUniform4fv(uniform(name).location, count, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::mat2 arr[])
{
    glUniformMatrix2fv(uniform(name).location, count, GL_FALSE, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::mat3 arr[])
{
    glUniformMatrix3fv(uniform(name).location, count, GL_FALSE, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::mat4 arr[])
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
    std::for_each(begin(uniforms), end(uniforms), [&](auto& attr) { out << attr.debugPrint() << endl; });

    out << "** Shader attributes **" << endl;
    std::for_each(begin(attributes), end(attributes), [&](auto& attr) { out << attr.debugPrint() << endl; });

    return out.str();
}

};
