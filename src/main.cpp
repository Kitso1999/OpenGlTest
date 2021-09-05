#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

#define GLEW_NO_GLU //To not include glu. It's deprecated
#include "GL/glew.h"
#include "GLFW/glfw3.h"

void checkShaderCompilationError(GLuint shader){
    GLint hasCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if(hasCompiled)
        return;
    GLint shaderType;
    glGetShaderiv(shader, GL_SHADER_TYPE, &shaderType);
    std::string shaderTypeString = (shaderType == GL_VERTEX_SHADER) ? "Vertex Shader" : "Fragment Shader";
    char infoLog[1024];
    GLsizei logLength;
    glGetShaderInfoLog(shader, 1024, &logLength, infoLog);
    std::cerr << shaderTypeString << " error: " << infoLog << std::endl;
}

void checkProgramLinkError(GLuint program){
    GLint hasLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &hasLinked);
    if(hasLinked)
        return;
    char infoLog[1024];
    GLsizei logLength;
    glGetProgramInfoLog(program, 1024, &logLength, infoLog);
    std::cerr << "Program error: " << infoLog << std::endl;
}

int main(){
#ifndef OPENGL_TEST_DEBUG_MODE
    int processID = fork();
    if(processID == -1){
        std::cerr << "Error ocured during fork()" << std::endl;
        return 1;
    }
    if(processID != 0){
        return 0;
    }
#endif
    if(glfwInit()!=GLFW_TRUE){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "OpenGL Test", NULL, NULL);

    if(window == NULL){
        std::cerr << "Failed to create a window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit()!=GLEW_OK){
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    float triangleBuffer[]{
        -0.75, -0.75,
        0.75, -0.75,
        0.0, 0.75,
        0.98, 0.36
    };

    unsigned int triangleIndexBuffer[]{
        0, 1, 2, 3
    };

    std::ifstream vertShaderIFS("../shaders/sample.vert");
    std::ifstream fragShaderIFS("../shaders/sample.frag");

    std::string IFShelper;

#ifdef OPENGL_TEST_DEBUG_MODE
    std::cout << vertShaderIFS.is_open() << std::endl;
    std::cout << fragShaderIFS.is_open() << std::endl;
#endif

    std::string vertShaderString = "", fragShaderString = "";

    while(std::getline(vertShaderIFS, IFShelper)){
        vertShaderString += IFShelper + '\n';
    }

    while(std::getline(fragShaderIFS, IFShelper)){
        fragShaderString += IFShelper + '\n';
    }

    vertShaderIFS.close();
    fragShaderIFS.close();

    const char* vertShaderSource = vertShaderString.c_str();
    const char* fragShaderSource = fragShaderString.c_str();

#ifdef OPENGL_TEST_DEBUG_MODE
    std::cout << vertShaderString << std::endl;
    std::cout << fragShaderString << std::endl;
#endif

    GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShaderID, 1, &vertShaderSource, NULL);
    glShaderSource(fragShaderID, 1, &fragShaderSource, NULL);

    glCompileShader(vertShaderID);
    glCompileShader(fragShaderID);
    
#ifdef OPENGL_TEST_DEBUG_MODE
    checkShaderCompilationError(vertShaderID);
    checkShaderCompilationError(fragShaderID);
#endif

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertShaderID);
    glAttachShader(programID, fragShaderID);

    glLinkProgram(programID);

    glUseProgram(programID);

#ifdef OPENGL_TEST_DEBUG_MODE
    checkProgramLinkError(programID);
#endif

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleBuffer), triangleBuffer, GL_STATIC_READ);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndexBuffer), triangleIndexBuffer, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0, 0.4, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLE_STRIP, sizeof(triangleIndexBuffer)/sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(programID);
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();
}