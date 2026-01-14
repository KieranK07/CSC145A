#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera global for callbacks (simple implementation)
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

int main() {
    // 1. Setup Window
    Window appWindow(SCR_WIDTH, SCR_HEIGHT, "3D Engine C++");
    glfwSetInputMode(appWindow.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(appWindow.getNativeWindow(), mouse_callback);

    // 2. Setup Shaders
    Shader ourShader("assets/shaders/model_loading.vs", "assets/shaders/model_loading.fs");

    // 3. Load Model
    Model ourModel("assets/models/backpack/backpack.obj");

    // 4. Render Loop
    while (!appWindow.shouldClose()) {
        // Time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        camera.ProcessKeyboard(appWindow.getNativeWindow(), deltaTime);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        // View/Projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)appWindow.getWidth() / (float)appWindow.getHeight(), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Render Model
        ourModel.Draw(ourShader);

        appWindow.swapBuffers();
        appWindow.pollEvents();
    }

    return 0;
}