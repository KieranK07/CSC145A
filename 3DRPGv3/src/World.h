#pragma once
#include "Renderer.h"
#include "Player.h"
#include "Texture.h"
#include <vector>
#include <memory>
#include <string>

struct AABB { glm::vec3 position; glm::vec3 size; };

class World {
public:
    std::unique_ptr<Player> player;
    std::unique_ptr<Texture> groundTexture;

    World();
    ~World();
    void Update(float dt);
    void Draw(Renderer& renderer, Shader& shader);
private:
    static constexpr AABB GroundCollider{glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f)};

    void ResolveCollisions(const glm::vec3& previousPosition);
    static bool AabbIntersect(const AABB& a, const AABB& b, glm::vec3& overlap);
};