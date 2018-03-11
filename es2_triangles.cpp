#include <iostream>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#include <GLES2/gl2.h>
#include "es2_triangles.h"
#include "SimpleShader.h"

using namespace std;

float vertices[] = {    
                        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
                    };

SimpleShader shader;
GLuint trianglesBufferId;

// Shader properties
GLint a_pos = -1;
GLint a_color = -1;

int main()
{
    if (!glfwInit())
    {
        cout << "glfwInit failed." << endl;
        return 1;
    }

    CreateWindow("es2_triangles");

    glfwTerminate();
    return 0;
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(shader.program);

    glBindBuffer(GL_ARRAY_BUFFER, trianglesBufferId);

    if (a_pos != -1)
    {
        glVertexAttribPointer(a_pos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
        glEnableVertexAttribArray(a_pos);
    }

    if (a_color != -1)
    {
        glVertexAttribPointer(a_color, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(a_color);
    }

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CreateVertexArray()
{
    
}

void CreateBuffers()
{
    
    glGenBuffers(1, &trianglesBufferId);
    
    glBindBuffer(GL_ARRAY_BUFFER, trianglesBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool InitShader()
{
    if (!shader.Init("simpleVertexShader.vert", "simlpeFragmentShader.frag"))
        return false;

    a_pos = glGetAttribLocation(shader.program, "a_pos");
    a_color = glGetAttribLocation(shader.program, "a_color");
    
    return true;
}

void CreateWindow(const char *title, int width /* = 800 */, int height /* = 600 */)
{
    // Version of OpenGL ES
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // Use OpenGL ES
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    glfwSetErrorCallback([](int error, const char *description){
        cout << "GLFW Error Callback" << endl;
        cout << error << ": " << description;
    });

    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        cout << "Cannot create window." << endl;
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (InitShader())
    {
        CreateBuffers();
    }
    glViewport(0, 0, width, height);
    
    while(!glfwWindowShouldClose(window))
    {
        int currentWidth;
        int currentHeight;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
        glViewport(0, 0, currentWidth, currentHeight);

        Render();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}
