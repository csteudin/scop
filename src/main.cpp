#include <iostream>

#include "../inc/scop.hpp"

#include "glad/glad.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


void framebufferSizeCallback(GLFWwindow *win, int width, int height)
{
    (void)win;
    glViewport(0, 0, width, height);
}

void proccessInput(GLFWwindow *win)
{
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}

int main()
{
    std::cout << "Hello W0rld\n" << std::endl;

    if (!glfwInit())
        return (1);   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    
    GLFWwindow *win;
    win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "ft_scop", nullptr, nullptr);
    if (!win)
        return (1); //glfwTerminate()
    int w, h;
    glfwGetWindowSize(win, &w, &h);
    std::cout << "Fenstergroesse: " << w << "x" << h << std::endl;
    glfwMakeContextCurrent(win);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return (1); //glfwTerminate()
    glfwSetFramebufferSizeCallback(win, framebufferSizeCallback);

    while(!glfwWindowShouldClose(win))
    {
        proccessInput(win);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    return (0);
}