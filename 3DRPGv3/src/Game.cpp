#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Initialize static members
int Game::WIDTH = 1920;
int Game::HEIGHT = 1080;

Game::Game() : window(nullptr), renderer(nullptr), world(nullptr), shader(nullptr), isPaused(false), escapePressed(false), wasPausedLastFrame(false), mouseDropFrames(0) {}

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

    // Get primary monitor and its video mode for fullscreen
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    
    // Set game dimensions to match monitor resolution
    WIDTH = mode->width;
    HEIGHT = mode->height;

    // Create fullscreen window
    window = glfwCreateWindow(WIDTH, HEIGHT, "RPG Scratch", primaryMonitor, NULL);
    if (!window) return false;
    glfwMakeContextCurrent(window);

    // Capture the mouse cursor (hide it)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

    glEnable(GL_DEPTH_TEST); 

    renderer = std::make_unique<Renderer>();
    world = std::make_unique<World>();

    // Updated path with "../" to look up one folder level
    shader = std::make_unique<Shader>("../assets/shaders/basic.vert", "../assets/shaders/basic.frag");

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    
    // Scale font size based on screen height for proper scaling
    float fontScale = HEIGHT / 1080.0f; // Base scale on 1080p
    if (fontScale < 1.0f) fontScale = 1.0f;
    io.FontGlobalScale = fontScale;
    
    ImGui::StyleColorsDark();
    
    // Scale UI elements
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(fontScale);
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

void Game::Run() {
    float lastFrame = 0.0f;
    mouseDropFrames = 1;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle pause input
        HandlePauseInput();
        
        // Handle pause state changes - store/restore velocity to prevent jolt
        if (!wasPausedLastFrame && isPaused) {
            pausedVelocity = world->player->Velocity;
            world->player->Velocity = glm::vec3(0.0f);
            mouseDropFrames = 1;
        } else if (wasPausedLastFrame && !isPaused) {
            world->player->Velocity = pausedVelocity;
            glfwSetCursorPos(window, WIDTH / 2.0, HEIGHT / 2.0);
            mouseDropFrames = 1; // drop one frame to absorb cursor warp
        }
        wasPausedLastFrame = isPaused;

        // While paused, swallow mouse deltas so none accumulate
        if (isPaused) {
            mouseDropFrames = 1;
        }
        
        // Only process game input and updates if not paused
        if (!isPaused) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            if (mouseDropFrames > 0) {
                glfwSetCursorPos(window, WIDTH / 2.0, HEIGHT / 2.0);
                --mouseDropFrames;
            } else {
                float xoffset = static_cast<float>(xpos - (WIDTH / 2.0));
                float yoffset = static_cast<float>((HEIGHT / 2.0) - ypos);
                world->player->ProcessMouseMovement(xoffset, yoffset);
                glfwSetCursorPos(window, WIDTH / 2.0, HEIGHT / 2.0);
            }
            // -----------------------------

            world->player->ProcessInput(window, dt);
            world->Update(dt);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // First-person camera positioned at player's eye level
        glm::vec3 eyeOffset = glm::vec3(0.0f, 1.5f, 0.0f); // Eye height above player feet
        glm::vec3 cameraPos = world->player->Position + eyeOffset;
        
        // Check if camera is colliding with objects, adjust if needed
        cameraPos = GetCollisionFreeCameraPos(cameraPos);
        
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

        // Render ImGui
        RenderImGui();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::RenderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Display FPS counter (scale with window size)
    float scale = HEIGHT / 1080.0f;
    if (scale < 1.0f) scale = 1.0f;
    
    ImGui::SetNextWindowPos(ImVec2(10 * scale, 10 * scale), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400 * scale, 120 * scale), ImGuiCond_Always);
    ImGui::Begin("Debug Info");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Player Position: (%.2f, %.2f, %.2f)", world->player->Position.x, world->player->Position.y, world->player->Position.z);
    ImGui::Text("Grounded: %s", world->player->IsGrounded ? "Yes" : "No");
    ImGui::End();

    // Pause menu
    if (isPaused) {
        DrawPauseMenuImGui();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::DrawPauseMenuImGui() {
    // Scale everything based on screen height
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
    
    // Center "Game is Paused" text
    const char* pauseText = "Game is Paused";
    float textWidth = ImGui::CalcTextSize(pauseText).x;
    ImGui::SetCursorPosX((windowWidth - textWidth) / 2.0f);
    ImGui::TextUnformatted(pauseText);
    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    
    // Center Resume button
    ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2.0f);
    if (ImGui::Button("Resume Game", ImVec2(buttonWidth, buttonHeight))) {
        isPaused = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    ImGui::Spacing();
    ImGui::Spacing();
    
    // Center Quit button
    ImGui::SetCursorPosX((windowWidth - buttonWidth) / 2.0f);
    if (ImGui::Button("Quit Game", ImVec2(buttonWidth, buttonHeight))) {
        glfwSetWindowShouldClose(window, true);
    }
    
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    
    // Center hint text
    const char* hintText = "Press ESC to toggle pause";
    float hintWidth = ImGui::CalcTextSize(hintText).x;
    ImGui::SetCursorPosX((windowWidth - hintWidth) / 2.0f);
    ImGui::TextUnformatted(hintText);
    
    ImGui::End();
}

void Game::HandlePauseInput() {
    int escapeState = glfwGetKey(window, GLFW_KEY_ESCAPE);
    
    // Toggle pause on Escape press (detect state change, not continuous press)
    if (escapeState == GLFW_PRESS && !escapePressed) {
        isPaused = !isPaused;
        
        if (isPaused) {
            // Unlock cursor when paused
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            // Lock cursor when unpaused
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        
        escapePressed = true;
    } else if (escapeState == GLFW_RELEASE) {
        escapePressed = false;
    }
}

void Game::HandlePauseMenuInput() {
    // ImGui handles this automatically
}

bool Game::IsCameraPositionValid(glm::vec3 cameraPos, float minDistance) {
    float capsuleRadius = 0.4f;
    
    // Check distance from player position (don't clip through player capsule)
    float distToPlayer = glm::distance(cameraPos, world->player->Position);
    if (distToPlayer < minDistance + capsuleRadius) {
        return false;
    }
    
    // Check floor collision (y > 0 + radius buffer)
    if (cameraPos.y < 0.2f) {
        return false;
    }
    
    // Check collision with platform
    AABB platform = world->platform;
    float dx = glm::max(fabsf(cameraPos.x - platform.position.x) - platform.size.x / 2.0f - minDistance, 0.0f);
    float dy = glm::max(fabsf(cameraPos.y - platform.position.y) - platform.size.y / 2.0f - minDistance, 0.0f);
    float dz = glm::max(fabsf(cameraPos.z - platform.position.z) - platform.size.z / 2.0f - minDistance, 0.0f);
    float dist = sqrtf(dx * dx + dy * dy + dz * dz);
    
    if (dist < minDistance) {
        return false;
    }
    
    return true;
}

glm::vec3 Game::GetCollisionFreeCameraPos(glm::vec3 targetPos) {
    // If target position is valid, use it
    if (IsCameraPositionValid(targetPos)) {
        return targetPos;
    }
    
    // Otherwise, gradually move camera towards player until valid position found
    glm::vec3 playerToCamera = targetPos - world->player->Position;
    glm::vec3 direction = glm::normalize(playerToCamera);
    float maxDistance = glm::length(playerToCamera);
    
    // Binary search for valid position
    float minDist = 0.5f;
    float maxDist = maxDistance;
    glm::vec3 bestPosition = world->player->Position + direction * minDist;
    
    for (int i = 0; i < 10; ++i) {
        float midDist = (minDist + maxDist) / 2.0f;
        glm::vec3 testPos = world->player->Position + direction * midDist;
        
        if (IsCameraPositionValid(testPos)) {
            bestPosition = testPos;
            minDist = midDist;
        } else {
            maxDist = midDist;
        }
    }
    
    return bestPosition;
}