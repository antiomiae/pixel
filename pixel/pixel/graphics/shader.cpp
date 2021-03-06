#include "graphics.h"
#include "shader_utils.h"
#include "../util/symbol_map.h"
#include "../util/util.h"
#include "../util/collections.h"
#include <pixel/error.h>
#include <sstream>
#include <fstream>
#include <iostream>

namespace pixel::graphics
{

using namespace std;

using pixel::collections::entries;


Shader::Shader(const char* vsPath, const char* fsPath, const vector<string>& transform_feedback_varyings)
{
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);

    load_shader(vs, vsPath);
    load_shader(fs, fsPath);

    if (!compile_shader(vs)) {
        log_shader_error(vs);
        pixel_error("Failed to compile vertex shader");
    };

    if (!compile_shader(fs)) {
        log_shader_error(fs);
        pixel_error("Failed to compile fragment shader");
    };

    auto program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    if (!transform_feedback_varyings.empty()) {
        std::vector<const char*> chars(transform_feedback_varyings.size());

        std::transform(
            begin(transform_feedback_varyings),
            end(transform_feedback_varyings),
            begin(chars),
            [](auto& s) {
                return &s.front();
            }
        );

        glTransformFeedbackVaryings(program, chars.size(), chars.data(), GL_INTERLEAVED_ATTRIBS);
    }

    if (!link_program(program)) {
        log_program_error(program);
        pixel_error("Failed to link program");
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    program_id_ = program;
    attribute_map_ = enumerate_program_attributes(program_id_);
    uniform_map_ = enumerate_program_uniforms(program_id_);
}


void Shader::activate()
{
    glUseProgram(program_id_);
}


void Shader::deactivate()
{
    glUseProgram(0);
}


Attribute Shader::attribute(const string& name) const
{
    try {
        auto attr = attribute_map_.at(name);
        return attr;
    } catch (exception& ex) {
        pixel_error("Unable to find attribute named `" + name + "`");
        throw ex;
    }

}


Attribute Shader::uniform(const string& name) const
{
    try {
        auto attr = uniform_map_.at(name);
        return attr;
    } catch (exception& ex) {
        pixel_error("Unable to find uniform named `" + name + "`");
        throw ex;
    }
}


void Shader::setUniform(const std::string& name, int v0)
{
    glProgramUniform1i(program_id_, uniform(name).location, v0);
    //glUniform1i(uniform(name).location, v0);
}


void Shader::setUniformArray(const std::string& name, int count, const int* arr)
{
    glProgramUniform1iv(program_id_, uniform(name).location, count, arr);
}


void Shader::setUniform(const std::string& name, int v0, int v1)
{
    glProgramUniform2i(program_id_, uniform(name).location, v0, v1);
}


void Shader::setUniform(const std::string& name, int v0, int v1, int v2)
{
    glProgramUniform3i(program_id_, uniform(name).location, v0, v1, v2);
}


void Shader::setUniform(const std::string& name, float v0)
{
    glProgramUniform1f(program_id_, uniform(name).location, v0);
}


void Shader::setUniform(const std::string& name, float v0, float v1)
{
    glProgramUniform2f(program_id_, uniform(name).location, v0, v1);
}

void Shader::setUniform(const std::string& name, const glm::ivec2& value)
{
    glProgramUniform2iv(program_id_, uniform(name).location, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::vec2& value)
{
    glProgramUniform2fv(program_id_, uniform(name).location, 1, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, float v0, float v1, float v2)
{
    glProgramUniform3f(program_id_, uniform(name).location, v0, v1, v2);
}


void Shader::setUniform(const std::string& name, const glm::vec3& value)
{
    glProgramUniform3fv(program_id_, uniform(name).location, 1, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, float v0, float v1, float v2, float v3)
{
    glProgramUniform4f(program_id_, uniform(name).location, v0, v1, v2, v3);
}


void Shader::setUniform(const std::string& name, const glm::vec4& value)
{
    glProgramUniform4fv(program_id_, uniform(name).location, 1, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, const glm::mat2& value)
{
    glProgramUniformMatrix2fv(program_id_, uniform(name).location, 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, const glm::mat3& value)
{
    glProgramUniformMatrix3fv(program_id_, uniform(name).location, 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::setUniform(const std::string& name, const glm::mat4& value)
{
    float mat[16];
    std::memcpy(&mat, glm::value_ptr(value), sizeof(mat));
    glProgramUniformMatrix4fv(program_id_, uniform(name).location, 1, GL_FALSE, mat);
}


void Shader::setUniformArray(const std::string& name, const int count, const float arr[])
{
    glProgramUniform1fv(program_id_, uniform(name).location, count, arr);
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::vec2 arr[])
{
    glProgramUniform2fv(program_id_, uniform(name).location, count, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::vec3 arr[])
{
    glProgramUniform3fv(program_id_, uniform(name).location, count, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::vec4 arr[])
{
    glProgramUniform4fv(program_id_, uniform(name).location, count, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::mat2 arr[])
{
    glProgramUniformMatrix2fv(program_id_, uniform(name).location, count, GL_FALSE, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::mat3 arr[])
{
    glProgramUniformMatrix3fv(program_id_, uniform(name).location, count, GL_FALSE, glm::value_ptr(arr[0]));
}


void Shader::setUniformArray(const std::string& name, const int count, const glm::mat4 arr[])
{
    glProgramUniformMatrix4fv(program_id_, uniform(name).location, count, GL_FALSE, glm::value_ptr(arr[0]));
}


std::string Shader::debugPrint() const
{
    auto uniforms = entries(uniform_map_);
    std::vector<Attribute> attributes = entries(attribute_map_);

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
