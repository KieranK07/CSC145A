#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
    Renderer* renderer;
    World* world;
    Shader* shader;
    bool isPaused;
    bool escapePressed;
    bool mousePressed;
    
    void HandlePauseInput();
    void HandlePauseMenuInput();
    void DrawPauseMenuImGui();
    void RenderImGui();
    glm::vec3 GetCollisionFreeCameraPos(glm::vec3 targetPos);
    bool IsCameraPositionValid(glm::vec3 cameraPos, float minDistance = 0.3f);
};