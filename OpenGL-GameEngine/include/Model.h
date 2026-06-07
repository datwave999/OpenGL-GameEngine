#pragma once

#include <vector>
#include <string>

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

    // Rendering Each Node
    void Render(Shader* shader);

private:
    std::vector<ModelNode> nodes;

    // Stores the directory of the loaded file (used later by Assimp to find textures)
    std::string directory;

    // Internal helper for Constructor 2
    void loadModel(const std::string& path);
};