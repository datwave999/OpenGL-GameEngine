#pragma once

#include <vector>
#include <string>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

struct ModelNode {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};

class Model {
public:
    // Manual Creating
    Model(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
    // Importing
    Model(const std::string& path);

    // Rendering The Model 
    void Render(Shader* shader);


private:
    std::vector<ModelNode> nodes;
    // Stores the directory of the loaded file (used later by Assimp to find textures)
    std::string directory;

    // Internal helpers for assimp
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};