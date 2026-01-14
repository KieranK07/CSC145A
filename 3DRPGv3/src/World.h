#pragma once
#include "Renderer.h"
#include "Player.h"
#include "Texture.h"
#include <vector>
#include <memory>

struct AABB { glm::vec3 position; glm::vec3 size; };

class World {
public:
    std::unique_ptr<Player> player;
    AABB platform;
    std::unique_ptr<Texture> groundTexture;
    std::unique_ptr<Texture> platformTexture;

    World();
    ~World();
    void Update(float dt);
    void Draw(Renderer& renderer, Shader& shader);
private:
    bool CheckCollision(Player* p, AABB box);
};