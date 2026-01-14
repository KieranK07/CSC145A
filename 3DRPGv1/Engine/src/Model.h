#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Mesh.h" // Define a simple Mesh struct/class with Draw()

class Model {
public:
    Model(const char* path) { loadModel(path); }
    void Draw(Shader &shader);
    
    // Position manipulation
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
    
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};