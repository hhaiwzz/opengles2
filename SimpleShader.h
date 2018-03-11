#ifndef _SIMLE_SHADER_
#define _SIMLE_SHADER_

#include <GLES2/gl2.h>

class SimpleShader
{
public:
    GLuint program;
    // Load vertex shader and fragment shader
    bool Init(const char *vertexShaderFile, const char *fragmentShaderFile);
private:
    GLuint vertexShaderId;
    GLuint fragmentShaderId;
    GLuint LoadShader(const char *shaderFile, GLenum type);
    bool LoadProgram();
};

#endif