#include "Model.h"
#include <iostream>


Model::Model(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) {
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

void Model::processNode(aiNode* node, const aiScene* scene) {

}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    return nullptr;
}
