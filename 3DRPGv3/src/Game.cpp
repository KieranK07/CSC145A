#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// initialize
int Game::WIDTH = 1920;
int Game::HEIGHT = 1080;

Game::Game() : window(nullptr), renderer(nullptr), world(nullptr), shader(nullptr), isPaused(false), escapePressed(false), equalPressed(false) {}

Game::~Game() { 
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate(); 
}

bool Game::Init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // pull monitor info for fullscreen
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    
    WIDTH = mode->width;
    HEIGHT = mode->height;

    // create the fullscreen window
    window = glfwCreateWindow(WIDTH, HEIGHT, "RPG Scratch", primaryMonitor, NULL);
    if (!window) return false;
    glfwMakeContextCurrent(window);

    // capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

    glEnable(GL_DEPTH_TEST); 

    renderer = std::make_unique<Renderer>();
    world = std::make_unique<World>();

    // make da shader
    shader = std::make_unique<Shader>("../assets/shaders/basic.vert", "../assets/shaders/basic.frag");

    // setup imgui for text
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // scale font to window size
    float fontScale = HEIGHT / 1080.0f; // base scale 1080p
    if (fontScale < 1.0f) fontScale = 1.0f;
    io.FontGlobalScale = fontScale;
    
    ImGui::StyleColorsDark();
    
    // scale the ui
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(fontScale);
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

// main game loop
void Game::Run() {
    float lastFrame = 0.0f;
    bool firstMouse = true;
    float lastX = WIDTH / 2.0f;
    float lastY = HEIGHT / 2.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // handle pause input
        HandlePauseInput();
        
        // handle flying mode toggle
        int equalState = glfwGetKey(window, GLFW_KEY_EQUAL);
        if (equalState == GLFW_PRESS && !equalPressed) {
            world->player->IsFlying = !world->player->IsFlying;
            equalPressed = true;
        } else if (equalState == GLFW_RELEASE) {
            equalPressed = false;
        }
        
        // make sure mouse only affects camera when the game is not paused
        if (!isPaused) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if (firstMouse) {
                lastX = static_cast<float>(xpos);
                lastY = static_cast<float>(ypos);
                firstMouse = false;
            }
            float xoffset = static_cast<float>(xpos) - lastX;
            float yoffset = lastY - static_cast<float>(ypos);
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            world->player->ProcessMouseMovement(xoffset, yoffset);
            // -----------------------------

            world->player->ProcessInput(window, dt);
            world->Update(dt);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // fps camera
        glm::vec3 eyeOffset = glm::vec3(0.0f, 1.5f, 0.0f); // camera height
        glm::vec3 cameraPos = world->player->Position + eyeOffset;
        
        glm::mat4 view = glm::lookAt(
            cameraPos, 
            cameraPos + world->player->Front,
            world->player->Up
        );

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        shader->Use();
        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);

        world->Draw(*renderer, *shader);

        // render imgui
        RenderImGui();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::RenderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // display fps
    float scale = HEIGHT / 1080.0f;
    if (scale < 1.0f) scale = 1.0f;
    
    ImGui::SetNextWindowPos(ImVec2(10 * scale, 10 * scale), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400 * scale, 120 * scale), ImGuiCond_Always);
    ImGui::Begin("Debug Info");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Player Position: (%.2f, %.2f, %.2f)", world->player->Position.x, world->player->Position.y, world->player->Position.z);
    ImGui::Text("Grounded: %s", world->player->IsGrounded ? "Yes" : "No");
    ImGui::Text("Flying: %s (Press = to toggle)", world->player->IsFlying ? "Yes" : "No");
    ImGui::End();

    // pause menu
    if (isPaused) {
        DrawPauseMenuImGui();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// draw the pause menu
void Game::DrawPauseMenuImGui() {
    float scale = HEIGHT / 1080.0f;
    if (scale < 1.0f) scale = 1.0f;
    
    float windowWidth = 500 * scale;
    float windowHeight = 400 * scale;
    float buttonWidth = 300 * scale;
    float buttonHeight = 60 * scale;
    
    ImGui::SetNextWindowPos(ImVec2(WIDTH / 2.0f - windowWidth / 2.0f, HEIGHT / 2.0f - windowHeight / 2.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.95f);
    
    ImGui::Begin("PAUSED", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    
    ImGui::SetCursorPosY(50 * scale);
    
    // create centered title
    const char* pauseText = "Game is Paused";
    float textWidth = ImGui::CalcTextSize(pauseText).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) / 2.0f);
    ImGui::TextUnformatted(pauseText);
    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    
    // center Resume button
    ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2.0f);
    if (ImGui::Button("Resume Game", ImVec2(buttonWidth, buttonHeight))) {
        isPaused = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    ImGui::Spacing();
    ImGui::Spacing();

    // center Quit button
    ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2.0f);
    if (ImGui::Button("Quit Game", ImVec2(buttonWidth, buttonHeight))) {
        glfwSetWindowShouldClose(window, true);
    }
    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    
    // center hint text
    const char* hintText = "Press ESC to toggle pause";
    float hintWidth = ImGui::CalcTextSize(hintText).x;
    ImGui::SetCursorPosX((windowWidth - hintWidth) / 2.0f);
    ImGui::TextUnformatted(hintText);
    
    ImGui::End();
}

void Game::HandlePauseInput() {
    int escapeState = glfwGetKey(window, GLFW_KEY_ESCAPE);
    
    // toggle esc key
    if (escapeState == GLFW_PRESS && !escapePressed) {
        isPaused = !isPaused;
        
        if (isPaused) {
            // unlock cursor when paused
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            // lock cursor when unpaused
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        
        escapePressed = true;
    } else if (escapeState == GLFW_RELEASE) {
        escapePressed = false;
    }
}

