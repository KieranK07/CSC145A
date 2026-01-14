#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Player {
public:
    // Transform Variables
    glm::vec3 Position;
    glm::vec3 Velocity;
    glm::vec3 Size;
    bool IsGrounded;

    // Camera/Rotation Variables
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;
    
    float Yaw;
    float Pitch;

    Player();
    void ProcessInput(GLFWwindow* window, float dt);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void Update(float dt);

private:
    void UpdateCameraVectors();
};