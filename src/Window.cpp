#include "../inc/scop.hpp"

/*  WINDOW::WINDOW
    - starts GLFW, asks for an OpenGL 3.3 core context, creates the window, enables VSync and
      loads GLAD
    - enables the depth test (near surfaces hide far ones) and sets the viewport + resize
      callback
    - on any failure it cleans up and throws a runtime_error
*/
Window::Window(int width, int height, const std::string &title)
{
    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    _win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!_win){
        glfwTerminate();
        throw std::runtime_error("glfwCreateWindow failed");
    }
    glfwMakeContextCurrent(_win);
    glfwSwapInterval(1); //VSYNC
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwDestroyWindow(_win);
        glfwTerminate();
        throw std::runtime_error("gladloadGLLoader failed");
    }
    glEnable(GL_DEPTH_TEST);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(_win, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glfwSetFramebufferSizeCallback(_win, framebufferSizeCallback);
    LOG("! Window Created");
}

/*  WINDOW::~WINDOW
    - destroys the window and shuts GLFW down
*/
Window::~Window()
{
    glfwDestroyWindow(_win);
    glfwTerminate();
    LOG("! Window Destroyed");
}

/*  WINDOW::SHOULDCLOSE
    - true as soon as the window is closed (X button or ESC)
*/
bool Window::shouldClose() const
{
    return (glfwWindowShouldClose(_win));
}

/*  WINDOW::SWAPBUFFERANDPOLLEVENT
    - shows the finished frame (swap front / back buffer)
    - handles the pending events: this is where the key and scroll callbacks are called
*/
void Window::swapBufferAndPollEvent()
{
    glfwSwapBuffers(_win);
    glfwPollEvents();
}

/*  WINDOW::GETASPECTRATIO
    - width / height of the framebuffer, needed by the projection matrix so the image is not
      distorted
    - returns 1 for height 0 (minimized window), so there is no division by 0
*/
float Window::getAspectRatio() const
{
    int w, h;
    glfwGetFramebufferSize(_win, &w, &h);
    return (h == 0) ? 1.0f : static_cast<float>(w) / static_cast<float>(h);
}

/*  WINDOW::GETHANDLE
    - the raw GLFW window pointer, for glfw calls outside of this class (input, callbacks,
      title)
*/
GLFWwindow *Window::getHandle() const
{
    return (_win);
}

//private

/*  WINDOW::FRAMEBUFFERSIZECALLBACK
    - called by GLFW when the window is resized: adapts the OpenGL viewport to the new size
*/
void Window::framebufferSizeCallback(GLFWwindow *win, int width, int height)
{
    (void)win;
    glViewport(0, 0, width, height);
}

