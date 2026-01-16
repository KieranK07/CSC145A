#include "Player.h"

Player::Player() {
    Position = glm::vec3(0.0f, 10.0f, 5.0f);
    Velocity = glm::vec3(0.0f);
    Size = glm::vec3(1.0f);
    HalfExtents = glm::vec3(0.35f, 0.9f, 0.35f);
    IsGrounded = false;
    IsFlying = true;

    // defaults for the camera
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Yaw = -90.0f; // start looking along -z
    Pitch = 0.0f;
    UpdateCameraVectors();
}

void Player::ProcessMouseMovement(float xoffset, float yoffset) {
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // clamping so no flip
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    UpdateCameraVectors();
}

void Player::UpdateCameraVectors() {
    // calc front vectors
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // calc right and up vectors
    Right = glm::normalize(glm::cross(Front, WorldUp)); 
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Player::ProcessInput(GLFWwindow* window, float dt) {
    float speed = 5.0f;
    
    if (IsFlying) {
        // Flying mode: full 3D movement
        glm::vec3 direction = glm::vec3(0.0f);
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction += Front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction -= Front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction -= Right;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction += Right;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) direction += WorldUp;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) direction -= WorldUp;
        
        if (glm::length(direction) > 0) {
            direction = glm::normalize(direction);
            Velocity = direction * speed;
        } else {
            Velocity = glm::vec3(0.0f);
        }
    } else {
        // Normal mode: ground-based movement
        glm::vec3 forward = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
        glm::vec3 right = glm::normalize(glm::vec3(Right.x, 0.0f, Right.z));

        glm::vec3 direction = glm::vec3(0.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction += forward;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction -= forward;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction -= right;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction += right;

        if (glm::length(direction) > 0) {
            direction = glm::normalize(direction);
            Velocity.x = direction.x * speed;
            Velocity.z = direction.z * speed;
        } else if (IsGrounded) {
            Velocity.x = 0;
            Velocity.z = 0;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && IsGrounded) {
            Velocity.y = 5.0f;
            IsGrounded = false;
        }
    }
}

void Player::Update(float dt) {
    // always assume airborne until collisions resolve
    IsGrounded = false;

    // gravity settings (only when not flying)
    if (!IsFlying) {
        Velocity.y -= 15.0f * dt;
    }

    // apply velocity to pos
    Position += Velocity * dt;
}