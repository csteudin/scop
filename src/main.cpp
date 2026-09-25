#include "../inc/scop.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void proccessInput(GLFWwindow *win, Vec3 &objectPosition, float &manualRotation, float deltaTime)
{
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
    float speed = 2.0f * deltaTime;
    float rotSpeed = 2.0f * deltaTime;

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        objectPosition.z -= speed;
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        objectPosition.z += speed;
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        objectPosition.x -= speed;
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        objectPosition.x += speed;
    if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
        manualRotation -= rotSpeed;
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS)
        manualRotation += rotSpeed;
}

Vec3 getObjectCenter(Parser &parser)
{
    Vec3 minP = parser.getVertices()[0].position;
    Vec3 maxP = parser.getVertices()[0].position;
    Vec3 objectCenter;
    
    for (const auto &v : parser.getVertices())
    {
        if (v.position.x < minP.x) minP.x = v.position.x;
        if (v.position.y < minP.y) minP.y = v.position.y;
        if (v.position.z < minP.z) minP.z = v.position.z;
        if (v.position.x > maxP.x) maxP.x = v.position.x;
        if (v.position.y > maxP.y) maxP.y = v.position.y;
        if (v.position.z > maxP.z) maxP.z = v.position.z;
    }

    objectCenter = (minP + maxP) * 0.5f;
    
    return (objectCenter);
}

float getObjectRadius(Parser &parser, const Vec3 &center)
{
    Vec3 minP = parser.getVertices()[0].position;
    Vec3 maxP = parser.getVertices()[0].position;

    for (const auto &v : parser.getVertices())
    {
        if (v.position.x < minP.x) minP.x = v.position.x;
        if (v.position.y < minP.y) minP.y = v.position.y;
        if (v.position.z < minP.z) minP.z = v.position.z;
        if (v.position.x > maxP.x) maxP.x = v.position.x;
        if (v.position.y > maxP.y) maxP.y = v.position.y;
        if (v.position.z > maxP.z) maxP.z = v.position.z;
    }
    (void)center;
    return ((maxP - minP).length() * 0.5f);
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
        Window window(WIN_WIDTH, WIN_HEIGHT, "ft_scop");
        Shader shader("shader/default.vert", "shader/default.frag"); 
        Parser parser(av[1]);
        Mesh mesh(parser.getVertices(), parser.getIndices());
        
        //init scene
        Vec3 objectCenter = getObjectCenter(parser);
        float objectRadius = getObjectRadius(parser, objectCenter);
        Camera camera(objectCenter + Vec3(0, 0, objectRadius * 2.5f), objectCenter);        
        float nearPlane = objectRadius * 0.01f;
        float farPlane = objectRadius * 10.0f;        

        Mat4 view = camera.getViewMatrix();
        Mat4 proj = Mat4::perspective(45.0f * 3.14159f / 180.0f, (float)WIN_WIDTH / WIN_HEIGHT, nearPlane, farPlane);
        
        //render loop
        float rotationAngle = 0;
        float manualRotation = 0;
        float lastFrame = 0;
        Vec3 objectPosition(0, 0, 0);
        while(!window.shouldClose())
        {
            //delta time
            float currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            //movement
            proccessInput(window.getHandle(), objectPosition, manualRotation, deltaTime);
            rotationAngle += 0.01f * deltaTime;
            

            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            //calculate view changes
            Mat4 toOrigin = Mat4::translate((objectCenter + objectPosition) * -1.0f);
            Mat4 rotation = Mat4::rotateY(rotationAngle + manualRotation);
            Mat4 backToPlace = Mat4::translate((objectCenter + objectPosition)); 
            Mat4 model = backToPlace * rotation * toOrigin;

            Mat4 mvp = proj * view * model;
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