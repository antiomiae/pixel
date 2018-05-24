//
//

#ifndef PIXEL_SHADER_UTILS_H
#define PIXEL_SHADER_UTILS_H

#include "common.h"
#include "graphics.h"

namespace pixel::graphics
{

void load_shader(GLuint shader, const char* path);
bool compile_shader(GLuint shader);
void log_shader_error(GLuint shader);
bool link_program(GLuint program);
void log_program_error(GLuint program);
int program_parameter(GLuint program, GLenum attribute);
AttributeMap enumerate_program_attributes(GLuint program);
AttributeMap enumerate_program_uniforms(GLuint program);

};

#endif //PIXEL_SHADER_UTILS_H
