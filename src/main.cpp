#include "../inc/scop.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>



void proccessInput(GLFWwindow *win, ObjectState &state, float deltaTime)
{
    float speed = cfg::MOVE_SPEED + deltaTime;
    
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) //ESC
        glfwSetWindowShouldClose(win, true);
    
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) //W
        state.position.z -= speed;
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) //S
        state.position.z += speed;
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) //A
        state.position.x -= speed;
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) //D
        state.position.x += speed;
    if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) //Q
        state.yaw -= cfg::KEY_ROT * deltaTime;
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) //E
        state.yaw += cfg::KEY_ROT * deltaTime;
    if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) //R
        state.position.y += speed;
    if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) //F
        state.position.y -= speed;
    
    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) //1 - RESET
    {
        state.position = Vec3(0, 0, 0); 
        state.yaw = state.pitch = 0;
        state.distance = state.startDistance;
    }
}

int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cerr << "Please use the program correct" << std::endl;
        std::cerr << "Usage: ./scop <file.obj>" << std::endl;
        return (1);
    }
    try
    {
        //init
        Parser parser(av[1]);
        Window window(WIN_WIDTH, WIN_HEIGHT, "ft_scop");
        Shader shader("shader/default.vert", "shader/default.frag"); 
        Mesh mesh(parser.getVertices(), parser.getIndices());
        
        //init scene with bounding box
        Vec3 objectCenter = parser.getBounds().center();
        float objectRadius = parser.getBounds().radius();
        Camera camera(objectCenter + Vec3(0, 0, objectRadius * 2.5f), objectCenter);        
        float nearPlane = objectRadius * 0.01f;
        float farPlane = objectRadius * 10.0f;        

        //create viewmatrix
        Mat4 view = camera.getViewMatrix();
        //Mat4 proj = Mat4::perspective(45.0f * 3.14159f / 180.0f, window.getAspectRatio(), nearPlane, farPlane);
        
        //render loop
        float lastFrame = static_cast<float>(glfwGetTime());
        ObjectState state;

        while(!window.shouldClose())
        {
            //delta time
            float currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            //movement
            proccessInput(window.getHandle(), state, deltaTime);
            state.autoRotation += cfg::AUTO_ROT * deltaTime;
            
            //clear buffer / CLEAR
            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            //calculate view changes / CALCULATE
            Mat4 toOrigin = Mat4::translate(objectCenter * -1.0f);
            Mat4 rotation = Mat4::rotateY(state.autoRotation + state.yaw);
            Mat4 backToPlace = Mat4::translate((objectCenter + state.position)); 
            Mat4 model = backToPlace * rotation * toOrigin;
            Mat4 proj = Mat4::perspective(45.0f * 3.14159f / 180.0f, window.getAspectRatio(), nearPlane, farPlane);
            Mat4 mvp = proj * view * model;

            // draw new image / DRAW
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
