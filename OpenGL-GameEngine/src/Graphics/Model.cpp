#include "Model.h"
#include <iostream>


Model::Model(Mesh* mesh, Material* material) {
    nodes.push_back({ mesh, material });
}

Model::Model(const std::string& path) {
    loadModel(path);
}

// --- The Render Loop ---
void Model::Render(Shader* shader) {
    for (const auto& node : nodes) {

        if (node.material) {
            node.material->Use(shader);
        }

        if (node.mesh) {
            node.mesh->RenderMesh();
        }
    }
}


void Model::loadModel(const std::string& path) {
}