#include "SimpleShader.h"
#include <iostream>
#include <fstream>

using namespace std;

bool SimpleShader::Init( const char *shaderIdFile, const char *fragmentShaderFile)
{
    if (shaderIdFile == nullptr || fragmentShaderFile == nullptr)
    {
        cout << "There was no shader to load." << endl;
        return false;
    }

    // Load verIdtex shader
    vertexShaderId = LoadShader(shaderIdFile, GL_VERTEX_SHADER);
    if (vertexShaderId == 0)
    {
        return false;
    }

    // Load fragment shader
    fragmentShaderId = LoadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
    if (fragmentShaderId == 0)
    {
        glDeleteShader(vertexShaderId);
        return false;
    }

    program = glCreateProgram();
    if (program == 0)
    {
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        cout << "Cannot create program." << endl;
        return false;
    }

    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_COMPILE_STATUS, &linked);
    if (linked != GL_TRUE)
    {
        GLint infoLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 0)
        {
            char *log = new char[infoLength];
            glGetProgramInfoLog(program, infoLength, nullptr, log);
            cout << "Error linking program: " << log << endl;
            delete[] log;
            glDeleteProgram(program);
            return false;
        }
    }

    return true;
}

GLuint SimpleShader::LoadShader(const char *shaderFile, GLenum type)
{
    // Open file and jump to the end of file
    ifstream file(shaderFile, ios::binary | ios::ate);
    if (!file.is_open())
    {
        cout << "Cannot load " << shaderFile << " file." << endl;
        return 0;
    }

    int size = file.tellg();
    if (size <= 0)
    {
        file.close();
        return 0;
    }

    file.seekg(0);

    char *shaderSource = new char[size];
    string shaderSrc;
    file.read(shaderSource, size);
    // shaderSource[size+1] = '\0';

    file.close();
    
    // Create shader in opengles2
    GLuint shaderId = glCreateShader(type);
    if (shaderId == 0)
    {
        cout << "Cannot create shader. (" << shaderFile << ")" << endl;
        delete[] shaderSource;
        return 0;
    }

    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    glCompileShader(shaderId);

    // Check compile status
    GLint compiled;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE)
    {
        GLint infoLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);

        if (infoLength > 0)
        {
            char *log = new char[infoLength];
            glGetShaderInfoLog(shaderId, infoLength, nullptr, log);
            cout << "Error compiling shader (" << shaderFile << "): " << log << endl;
            delete[] log;
            delete[] shaderSource;
        }

        glDeleteShader(shaderId);
        return 0;
    }

    delete[] shaderSource;

    return shaderId;
}