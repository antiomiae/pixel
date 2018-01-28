//
//

#ifndef GL_SHADER_H
#define GL_SHADER_H

#include "common.h"
#include "../math/math.h"
#include <unordered_map>
#include <string>

namespace pixel::graphics {

struct Attribute {
    GLuint index;
    GLint size;
    GLint location;
    GLenum type;
    std::string name;

    std::string debugPrint() const;
};

using AttributeMap = std::unordered_map<std::string, Attribute>;

class Shader {
public:
    GLuint _programId;
    AttributeMap _attributeMap;
    AttributeMap _uniformMap;

    std::string _debugName;

    Shader();
    Shader(const char *vs, const char *fs, const char *debugName = "unnamed shader");

    void activate();
    void deactivate();

    Attribute attribute(const std::string &name) const;
    Attribute uniform(const std::string &name) const;

    void setUniform(const std::string &name, int);
    void setUniform(const std::string &name, int, int);
    void setUniform(const std::string &name, int, int, int);

    void setUniform(const std::string &name, float);
    void setUniform(const std::string &name, float, float);
    void setUniform(const std::string &name, float, float, float);
    void setUniform(const std::string &name, float, float, float, float);

    void setUniform(const std::string &name, const glm::vec2 &value);
    void setUniform(const std::string &name, const glm::vec3 &value);
    void setUniform(const std::string &name, const glm::vec4 &value);

    void setUniform(const std::string &name, const glm::mat2 &value);
    void setUniform(const std::string &name, const glm::mat3 &value);
    void setUniform(const std::string &name, const glm::mat4 &value);

    void setUniformArray(const std::string &name, int count, const float *arr);
    void setUniformArray(const std::string &name, int count, const int *arr);

    void setUniformArray(const std::string &name, int count, const glm::vec2 *arr);
    void setUniformArray(const std::string &name, int count, const glm::vec4 *arr);
    void setUniformArray(const std::string &name, int count, const glm::vec3 *arr);

    void setUniformArray(const std::string &name, int count, const glm::mat2 *arr);
    void setUniformArray(const std::string &name, int count, const glm::mat3 *arr);
    void setUniformArray(const std::string &name, int count, const glm::mat4 *arr);

    std::string debugPrint() const;

};

}
#endif //GL_SHADER_H
