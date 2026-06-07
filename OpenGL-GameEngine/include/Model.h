#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

struct ModelNode {
    Mesh* mesh;
    Material* material;
};

class Model {
public:
    // Manual Creating
    Model(Mesh* mesh, Material* material);
    // Importing
    Model(const std::string& path);

    // Rendering The Model 
    void Render(Shader* shader);

    ~Model();

private:
    std::vector<ModelNode> nodes;
    // Stores the directory of the loaded file (used later by Assimp to find textures)
    std::string directory;

    // Internal helpers for assimp
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};