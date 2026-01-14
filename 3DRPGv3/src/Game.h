#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Renderer.h"
#include "World.h"
#include "Shader.h"

class Game {
public:
    // Screen size (will be set to monitor resolution)
    static int WIDTH;
    static int HEIGHT;

    Game();
    ~Game();
    bool Init();
    void Run();

private:
    GLFWwindow* window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<World> world;
    std::unique_ptr<Shader> shader;
    bool isPaused;
    bool escapePressed;
    bool wasPausedLastFrame;
    int mouseDropFrames;
    glm::vec3 pausedVelocity;
    
    void HandlePauseInput();
    void HandlePauseMenuInput();
    void DrawPauseMenuImGui();
    void RenderImGui();
    glm::vec3 GetCollisionFreeCameraPos(glm::vec3 targetPos);
    bool IsCameraPositionValid(glm::vec3 cameraPos, float minDistance = 0.3f);
};