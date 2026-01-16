#include "World.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace {
glm::vec3 GetAabbMin(const AABB& box) {
    return box.position - box.size * 0.5f;
}

glm::vec3 GetAabbMax(const AABB& box) {
    return box.position + box.size * 0.5f;
}
}

World::World() {
    player = std::make_unique<Player>();
    groundTexture = std::make_unique<Texture>("../assets/textures/ground.png");
}

World::~World() = default;

bool World::AabbIntersect(const AABB& a, const AABB& b, glm::vec3& overlap) {
    glm::vec3 aMin = GetAabbMin(a);
    glm::vec3 aMax = GetAabbMax(a);
    glm::vec3 bMin = GetAabbMin(b);
    glm::vec3 bMax = GetAabbMax(b);

    overlap.x = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
    overlap.y = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);
    overlap.z = std::min(aMax.z, bMax.z) - std::max(aMin.z, bMin.z);
    return overlap.x > 0.0f && overlap.y > 0.0f && overlap.z > 0.0f;
}

void World::ResolveCollisions(const glm::vec3& previousPosition) {
    AABB playerBox{player->Position, player->HalfExtents * 2.0f};

    glm::vec3 overlap;
    if (!AabbIntersect(playerBox, GroundCollider, overlap)) return;

    int axis = 0;
    float minOverlap = overlap.x;
    if (overlap.y < minOverlap) { axis = 1; minOverlap = overlap.y; }
    if (overlap.z < minOverlap) { axis = 2; minOverlap = overlap.z; }

    glm::vec3 correction(0.0f);
    if (axis == 0) {
        correction.x = (playerBox.position.x >= GroundCollider.position.x) ? overlap.x : -overlap.x;
        player->Velocity.x = 0.0f;
    } else if (axis == 1) {
        correction.y = (playerBox.position.y >= GroundCollider.position.y) ? overlap.y : -overlap.y;
        player->Velocity.y = 0.0f;
        if (correction.y > 0.0f && previousPosition.y >= GetAabbMax(GroundCollider).y) {
            player->IsGrounded = true;
        }
    } else {
        correction.z = (playerBox.position.z >= GroundCollider.position.z) ? overlap.z : -overlap.z;
        player->Velocity.z = 0.0f;
    }

    playerBox.position += correction;
    player->Position = playerBox.position;
}

void World::Update(float dt) {
    glm::vec3 previousPosition = player->Position;
    player->Update(dt);

    if (!player->IsFlying) {
        ResolveCollisions(previousPosition);
    }
}

void World::Draw(Renderer& renderer, Shader& shader) {
    renderer.DrawCubeTextured(shader, glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f), *groundTexture, 10.0f);
}