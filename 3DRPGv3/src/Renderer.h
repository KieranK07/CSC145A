#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

class Renderer {
public:
    Renderer();
    void DrawCube(Shader& shader, glm::vec3 position, glm::vec3 size, glm::vec3 color);
    void DrawCubeTextured(Shader& shader, glm::vec3 position, glm::vec3 size, Texture& texture);
    void DrawCubeTextured(Shader& shader, glm::vec3 position, glm::vec3 size, Texture& texture, float tilingFactor);
private:
    unsigned int cubeVAO, cubeVBO;
};