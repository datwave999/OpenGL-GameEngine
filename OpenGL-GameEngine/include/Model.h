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

class AssetContainer;

struct ModelNode {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};

class Model {
public:
    Model(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
    Model(const std::string& path, AssetContainer* assets);

    void Render(Shader* shader);

private:
    std::vector<ModelNode> nodes;
    std::string directory;

    AssetContainer* assetManager;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    ModelNode processMesh(aiMesh* mesh, const aiScene* scene);
};