#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

class Renderer {
public:
    Renderer();
    void DrawCubeTextured(Shader& shader, glm::vec3 position, glm::vec3 size, Texture& texture, float tilingFactor = 1.0f);
private:
    unsigned int cubeVAO, cubeVBO;
};