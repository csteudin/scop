#pragma once

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window    {
    public:
        Window(int width, int height, const std::string &title);
        ~Window();

        bool shouldClose() const;
        void swapBufferAndPollEvent();
        GLFWwindow *getHandle() const;

    private:
        static void framebufferSizeCallback(GLFWwindow *win, int width, int height);
        GLFWwindow *_win;

};