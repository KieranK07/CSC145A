#include "World.h"

World::World() {
    player = new Player();
    // Create a platform at (2, 0.5, -2) with size (3, 1, 3)
    platform = { glm::vec3(2.0f, 0.5f, -2.0f), glm::vec3(3.0f, 1.0f, 3.0f) };
    
    // Load textures (will use white fallback if files don't exist)
    groundTexture = new Texture("../assets/textures/ground.png");
    platformTexture = new Texture("../assets/textures/box.png");
}

World::~World() {
    delete player;
    delete groundTexture;
    delete platformTexture;
}

bool World::CheckCollision(Player* p, AABB box) {
    // Capsule collision with AABB
    // Treat player as a capsule with radius 0.4f for collision
    float capsuleRadius = 0.4f;
    
    // Find closest point on AABB to capsule center
    float closestX = glm::clamp(p->Position.x, box.position.x - box.size.x/2, box.position.x + box.size.x/2);
    float closestY = glm::clamp(p->Position.y, box.position.y - box.size.y/2, box.position.y + box.size.y/2);
    float closestZ = glm::clamp(p->Position.z, box.position.z - box.size.z/2, box.position.z + box.size.z/2);
    
    // Distance from capsule center to closest point on box
    float dx = p->Position.x - closestX;
    float dy = p->Position.y - closestY;
    float dz = p->Position.z - closestZ;
    
    float distance = sqrtf(dx * dx + dy * dy + dz * dz);
    
    return distance < capsuleRadius;
}

void World::Update(float dt) {
    player->Update(dt);

    // Improved collision resolution with multiple iterations to handle all sides
    const float CAPSULE_RADIUS = 0.4f;
    const int COLLISION_ITERATIONS = 3;
    
    for (int iter = 0; iter < COLLISION_ITERATIONS; ++iter) {
        // Check collision with platform
        if (CheckCollision(player, platform)) {
            // Find the closest point on the AABB to the player's position
            float closestX = glm::clamp(player->Position.x, 
                platform.position.x - platform.size.x/2, 
                platform.position.x + platform.size.x/2);
            float closestY = glm::clamp(player->Position.y, 
                platform.position.y - platform.size.y/2, 
                platform.position.y + platform.size.y/2);
            float closestZ = glm::clamp(player->Position.z, 
                platform.position.z - platform.size.z/2, 
                platform.position.z + platform.size.z/2);
            
            // Calculate penetration vector
            float dx = player->Position.x - closestX;
            float dy = player->Position.y - closestY;
            float dz = player->Position.z - closestZ;
            float dist = sqrtf(dx * dx + dy * dy + dz * dz);
            
            if (dist < CAPSULE_RADIUS && dist > 0.001f) {
                // Normalize the penetration vector
                dx /= dist;
                dy /= dist;
                dz /= dist;
                
                // Push player out of collision
                float penetration = CAPSULE_RADIUS - dist;
                player->Position.x += dx * penetration;
                player->Position.y += dy * penetration;
                player->Position.z += dz * penetration;
                
                // Stop velocity in the direction of penetration
                float velDot = player->Velocity.x * dx + player->Velocity.y * dy + player->Velocity.z * dz;
                if (velDot < 0) {
                    player->Velocity.x -= velDot * dx;
                    player->Velocity.y -= velDot * dy;
                    player->Velocity.z -= velDot * dz;
                }
                
                // Set grounded if colliding from top
                if (dy > 0.7f && player->Velocity.y < 0.1f) {
                    player->IsGrounded = true;
                    player->Velocity.y = 0;
                }
            }
        }
    }
}

void World::Draw(Renderer& renderer, Shader& shader) {
    // Player is now invisible (it's a first-person character)
    // Only draw environment objects
    
    // Draw Platform with texture
    renderer.DrawCubeTextured(shader, platform.position, platform.size, *platformTexture);

    // Draw Floor with texture (repeat the texture for large floor)
    renderer.DrawCubeTextured(shader, glm::vec3(0,-0.5,0), glm::vec3(20, 1, 20), *groundTexture);
}