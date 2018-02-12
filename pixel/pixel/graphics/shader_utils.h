//
//

#ifndef PIXEL_SHADER_UTILS_H
#define PIXEL_SHADER_UTILS_H

#include "common.h"
#include "attribute.h"
#include "attribute.h"

namespace pixel::graphics
{

void loadShader(GLuint shader, const char* path);
bool compileShader(GLuint shader);
void logShaderError(GLuint shader);
bool linkProgram(GLuint program);
void logProgramError(GLuint program);
int programParameter(GLuint program, GLenum attribute);
AttributeMap enumerateProgramAttributes(GLuint program);
AttributeMap enumerateProgramUniforms(GLuint program);

};

#endif //PIXEL_SHADER_UTILS_H
