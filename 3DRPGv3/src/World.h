#pragma once
#include "Renderer.h"
#include "Player.h"
#include "Texture.h"
#include <vector>

struct AABB { glm::vec3 position; glm::vec3 size; };

class World {
public:
    Player* player;
    AABB platform;
    Texture* groundTexture;
    Texture* platformTexture;

    World();
    ~World();
    void Update(float dt);
    void Draw(Renderer& renderer, Shader& shader);
private:
    bool CheckCollision(Player* p, AABB box);
};