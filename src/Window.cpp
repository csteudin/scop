#include "../inc/scop.hpp"

Window::Window(int width, int height, const std::string &title)
{
    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    
    this->_win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!_win)
        throw std::runtime_error("glfwCreateWindow failed");
    glfwMakeContextCurrent(_win);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("gladloadGLLoader failed");
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(_win, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glfwSetFramebufferSizeCallback(_win, framebufferSizeCallback);
    LOG("! Window Created");
}

Window::~Window()
{
    glfwDestroyWindow(_win);
    glfwTerminate();
    LOG("! Window Destroyed");
}

bool Window::shouldClose() const
{
    return (glfwWindowShouldClose(_win));
}

void Window::swapBufferAndPollEvent()
{
    glfwSwapBuffers(_win);
    glfwPollEvents();
}

GLFWwindow *Window::getHandle() const
{
    return (_win);
}

//private
void Window::framebufferSizeCallback(GLFWwindow *win, int width, int height)
{
    (void)win;
    glViewport(0, 0, width, height);
}

