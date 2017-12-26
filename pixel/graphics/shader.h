//
//

#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <unordered_map>
#include <string>
#include "common.h"


namespace pixel::graphics {

struct Attribute {
    GLuint index;
    GLint size;
    GLenum type;
    char name[30];
};

using AttributeMap = std::unordered_map<std::string, Attribute>;

class Shader {
public:
    GLuint _programId;
    AttributeMap _attributeMap;
    AttributeMap _uniformMap;

    Shader();
    Shader(const char *vs, const char *fs);

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

private:

};

}
#endif //GL_SHADER_H
