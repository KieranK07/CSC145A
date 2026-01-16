#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Renderer.h"
#include "World.h"
#include "Shader.h"

class Game {
public:
    // screen size
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
    bool equalPressed;
    
    void HandlePauseInput();
    void DrawPauseMenuImGui();
    void RenderImGui();
};