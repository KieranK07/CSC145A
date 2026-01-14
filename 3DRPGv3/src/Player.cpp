#include "Player.h"

Player::Player() {
    Position = glm::vec3(0.0f, 2.0f, 0.0f);
    Velocity = glm::vec3(0.0f);
    Size = glm::vec3(1.0f);
    IsGrounded = false;

    // Camera Defaults
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Yaw = -90.0f; // Look forward (Z axis)
    Pitch = 0.0f;
    UpdateCameraVectors();
}

void Player::ProcessMouseMovement(float xoffset, float yoffset) {
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // Clamp Pitch so you don't flip upside down
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    UpdateCameraVectors();
}

void Player::UpdateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Calculate Right and Up vectors
    Right = glm::normalize(glm::cross(Front, WorldUp)); 
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Player::ProcessInput(GLFWwindow* window, float dt) {
    float speed = 5.0f;
    
    // Move on the XZ plane (Standard RPG walking)
    // We remove the Y component of Front so looking up doesn't make you fly up
    glm::vec3 forward = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    glm::vec3 right = glm::normalize(glm::vec3(Right.x, 0.0f, Right.z));

    glm::vec3 direction = glm::vec3(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction += forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction -= forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction += right;

    // Apply movement logic (only affect X/Z velocity)
    if (glm::length(direction) > 0) {
        direction = glm::normalize(direction);
        Velocity.x = direction.x * speed;
        Velocity.z = direction.z * speed;
    } else {
        Velocity.x = 0;
        Velocity.z = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && IsGrounded) {
        Velocity.y = 5.0f;
        IsGrounded = false;
    }
}

void Player::Update(float dt) {
    // Gravity
    Velocity.y -= 9.8f * dt;

    // Apply Velocity
    Position += Velocity * dt;

    // Simple Floor
    if (Position.y < 0.5f) {
        Position.y = 0.5f;
        Velocity.y = 0.0f;
        IsGrounded = true;
    }
}