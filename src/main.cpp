#include "../inc/scop.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void proccessInput(GLFWwindow *win)
{
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;

    if (ac < 2)
    {
        std::cerr << "Please use the program correct" << std::endl;
        std::cerr << "Usage: ./scop <file.obj>" << std::endl;
        return (1);
    }
    try
    {
        Window window(WIN_WIDTH, WIN_HEIGHT, "ft_scop");
        Shader shader("shader/default.vert", "shader/default.frag");
       
        Parser parser("resources/42.obj");

    

        //TESTING - - - - - - - -
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };

        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //- - - - - - - - - - - -


        while(!window.shouldClose())
        {
            proccessInput(window.getHandle());

            glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            window.swapBufferAndPollEvent();
            sleep(1);
            //glfwSwapInterval(1);
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}

/*
    PSEUDO CODE FOR RENDERING

    MAIN()
    - PARSE OBJFILE TODO:
    - INIT WINDOW
        - RENDER LOOP
            - CHECKINPUT
            - PUT NEW IMAGE
    - CLEAN
*/