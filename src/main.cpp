#include "../inc/scop.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

/*  PROCESSINPUT
    - polled once per frame: reads the keys that are HELD (WASD/R/F move, Q/E turn, 1 reset, ESC
      quit) and the left-mouse drag
    - only writes into the ObjectState, the matrices are built later in main()
    - every speed is multiplied by deltaTime, so the motion is the same at any FPS
*/
void processInput(GLFWwindow *win, ObjectState &state, float deltaTime)
{
    float speed = state.moveSpeed * deltaTime; // maybe tweak obj mov speed
    
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
    
    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double x, y;
        glfwGetCursorPos(win, &x, &y);
        if (state.dragging)
        {
            state.yaw += static_cast<float>(x - state.lastX) * cfg::MOUSE_SENS;
            state.pitch += static_cast<float>(y - state.lastY) * cfg::MOUSE_SENS;
        }
        state.lastX = x;
        state.lastY = y;
        state.dragging = true;
    }
    else
        state.dragging = false;

    if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) //1 - RESET
    {
        state.position = Vec3(0, 0, 0); 
        state.yaw = state.pitch = 0;
        state.distance = state.startDistance;
    }
}

/*  KEYCALLBACK
    - called by GLFW once per key event: handles the keys that count ONCE per press (T texture,
      L light, G wireframe)
    - only flips the booleans in the ObjectState, the next frame picks them up (T sets
      blendTarget, the fade itself is done in updateFade)
*/
void keyCallback(GLFWwindow *win, int key, int x, int action, int y)
{
    (void)x;
    (void)y;

    if (action != GLFW_PRESS)
        return ;

    ObjectState *state = static_cast<ObjectState *>(glfwGetWindowUserPointer(win));
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        state->blendTarget = (state->blendTarget == 0) ? 1 : 0;
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        state->light = (state->light == 0) ? 1 : 0;
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
        state->wireframe = (state->wireframe == 0) ? 1 : 0;
}

/*  SCROLLCALLBACK
    - mouse wheel = zoom: moves the camera distance by one zoomStep per wheel tick (scroll up =
      closer)
    - the limits (min / max distance) are applied with std::clamp in main()
*/
void scrollCallback(GLFWwindow *win, double x, double yOffset)
{
    (void)x;
    ObjectState *state = static_cast<ObjectState *>(glfwGetWindowUserPointer(win));
    state->distance -= static_cast<float>(yOffset) * state->zoomStep;
}

/*  UPDATEFPS
    - counts the frames and writes the result into the window title once per second
    - the static variables keep their value between the calls, so no extra state is needed in
      main()
*/
void updateFps(GLFWwindow *win, float deltaTime)
{
    static int frames = 0;
    static float timer = 1.0f;

    frames ++;
    timer += deltaTime;
    if (timer >= 1.0f)
    {
        glfwSetWindowTitle(win, ("ft_scop | FPS : " + std::to_string(frames)).c_str());
        frames = 0;
        timer = 0.0f;
    }
}

/*  UPDATEFADE
    - moves state.blend (0 = colors, 1 = texture) smoothly towards state.blendTarget, so
      pressing T fades instead of jumping
    - std::min / std::max stop exactly on the target, the value can never overshoot
*/
void updateFade(ObjectState &state, float deltaTime)
{
    float step = cfg::FADE_SPEED * deltaTime;

    if (state.blend < state.blendTarget)
        state.blend = std::min(state.blend + step, state.blendTarget);
    else if (state.blend > state.blendTarget)
        state.blend = std::max(state.blend - step, state.blendTarget);
}

/*  MAIN
    - loads the model, shader and texture (RAII: everything is freed automatically, even if an
      exception is thrown)
    - sizes the scene from the bounding box, so any model fits the camera
    - render loop: deltaTime -> update -> camera + matrices (model, view, proj) -> draw
*/
int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cerr << "Please use the program correct" << std::endl;
        std::cerr << "Usage: ./scop <file.obj> [texture.bmp]" << std::endl;
        return (1);
    }
    try
    {
        //init
        Parser parser(av[1]);
        Window window(WIN_WIDTH, WIN_HEIGHT, "ft_scop");
        Shader shader("shader/default.vert", "shader/default.frag"); 
        Mesh mesh(parser.getVertices(), parser.getIndices());
        Texture texture(ac > 2 ? av[2] : "textures/kitty.bmp");

        //init scene with bounds
        Vec3 objectCenter = parser.getBounds().center();
        float objectRadius = parser.getBounds().radius();
        float nearPlane = objectRadius * 0.01f;
        float farPlane = objectRadius * 10.0f;        
        Camera camera(objectCenter + Vec3(0, 0, objectRadius * 2.5f), objectCenter);        

        //render loop
        ObjectState state;
        state.distance = state.startDistance = objectRadius * cfg::CAM_DISTANCE;
        state.zoomStep  = objectRadius * 0.1f;
        state.moveSpeed = objectRadius * cfg::MOVE_SPEED_REL;
        glfwSetScrollCallback(window.getHandle(), scrollCallback);
        glfwSetWindowUserPointer(window.getHandle(), &state);
        glfwSetKeyCallback(window.getHandle(), keyCallback);

        float lastFrame = static_cast<float>(glfwGetTime());
        while(!window.shouldClose())
        {
            //get new time
            float currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            //update / fps-counter / key-callbacks / fade callback
            updateFps(window.getHandle(), deltaTime);
            processInput(window.getHandle(), state, deltaTime);
            if (!state.dragging)
                state.autoRotation += cfg::AUTO_ROT * deltaTime;
            updateFade(state, deltaTime);

            state.distance = std::clamp(state.distance, objectRadius * 1.2f, objectRadius * 6.0f);
            camera.position = objectCenter + Vec3(0, 0, state.distance);
            
            //calculate view changes / CALCULATE
            Mat4 toOrigin = Mat4::translate(objectCenter * -1.0f);
            Mat4 rotation = Mat4::rotateX(state.pitch) * Mat4::rotateY(state.autoRotation + state.yaw);
            Mat4 backToPlace = Mat4::translate((objectCenter + state.position)); 
            Mat4 model = backToPlace * rotation * toOrigin;
            Mat4 view = camera.getViewMatrix();
            Mat4 proj = Mat4::perspective(cfg::FOV_DEG * 3.14159f / 180.0f, window.getAspectRatio(), nearPlane, farPlane);
            Mat4 mvp = proj * view * model;
            
            //clear buffer / CLEAR
            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // draw new image / DRAW
            shader.use();
            shader.setMat4("uMVP", mvp);
            shader.setMat4("uModel", model);
            shader.setFloat("uLight", state.light ? 1.0f : 0.0f);
            texture.bind(0);
            shader.setInt("uTexture", 0);
            shader.setFloat("uBlend", state.blend);
            glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);
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
