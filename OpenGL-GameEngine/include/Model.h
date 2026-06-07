#pragma once

#include <vector>
#include <string>

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

// A model is made up of one or more meshes, each needing its own material.
struct ModelNode {
    Mesh* mesh;
    Material* material;
};

class Model {
public:
    // Constructor 1: Manual creation (Procedural shapes)
    Model(Mesh* mesh, Material* material);

    // Constructor 2: External Asset creation (Assimp)
    Model(const std::string& path);

    // Renders all sub-meshes with their respective materials
    void Render(Shader* shader);

private:
    std::vector<ModelNode> nodes;

    // Stores the directory of the loaded file (used later by Assimp to find textures)
    std::string directory;

    // Internal helper for Constructor 2
    void loadModel(const std::string& path);
};