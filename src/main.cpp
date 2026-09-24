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
       
        Parser parser(av[1]);
        
        Mesh mesh(parser.getVertices(), parser.getIndices());


        //TESTING - - - - - - - -
        Mat4 model = Mat4::identity();
        Mat4 view = Mat4::translate(Vec3(0, 0, -5));
        Mat4 proj = Mat4::perspective(45.0f * 3.14159f / 180.0f, (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 1000.0f);
        Mat4 mvp = proj * view * model;
        // - - - - - - - - - - -
        while(!window.shouldClose())
        {
            proccessInput(window.getHandle());

            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            shader.use();
            shader.setMat4("uMVP", mvp);
            mesh.draw();
            window.swapBufferAndPollEvent();
        }

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