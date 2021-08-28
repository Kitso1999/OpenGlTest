#include <iostream>
#include <unistd.h>

#define GLEW_NO_GLU //To not include glu. It's deprecated
#include "GL/glew.h"
#include "GLFW/glfw3.h"

int main(){
    int processID = fork();
    if(processID == -1){
        std::cerr << "Error ocured during fork()" << std::endl;
        return 1;
    }
    if(processID != 0){
        return 0;
    }
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

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2, 0.4, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}