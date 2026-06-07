#include "Model.h"
#include <iostream>

// --- Constructor 1: Manual Procedural Setup ---
Model::Model(Mesh* mesh, Material* material) {
    // Simply package the provided mesh and material into a node
    nodes.push_back({ mesh, material });
}

// --- Constructor 2: File Loading Setup ---
Model::Model(const std::string& path) {
    loadModel(path);
}

// --- The Render Loop ---
void Model::Render(Shader* shader) {
    // Loop through every piece of this model
    for (const auto& node : nodes) {

        // 1. Bind the specific material for this piece
        if (node.material) {
            node.material->Use(shader);
        }

        // 2. Draw the geometry
        if (node.mesh) {
            node.mesh->RenderMesh();
        }
    }
}

// --- Internal Assimp Scaffolding ---
void Model::loadModel(const std::string& path) {
    // We will fill this in next!
    std::cout << "Preparing to load Assimp model from: " << path << std::endl;
}