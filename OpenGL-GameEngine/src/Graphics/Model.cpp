#include <iostream>

#include "Model.h"
#include "AssetContainer.h"


Model::Model(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) : assetManager(nullptr) {
    nodes.push_back({ mesh, material });
}

Model::Model(const std::string& path, AssetContainer* assets, bool flipUVs) : assetManager(assets) {
    loadModel(path, flipUVs);
}

// --- The Render Loop ---
void Model::Render(Shader* shader) {
    for (const auto& node : nodes) {
        if (node.material) node.material->Use(shader);
        if (node.mesh) node.mesh->RenderMesh();
    }
}

void Model::loadModel(const std::string& path, bool flipUVs) {

    Assimp::Importer importer;
    unsigned int importFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals;
    if (flipUVs) importFlags |= aiProcess_FlipUVs;
    const aiScene* scene = importer.ReadFile(path, importFlags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process all the meshes attached to this specific node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        nodes.push_back(processMesh(mesh, scene));
    }
    // Recursively tell all children nodes to do the same thing
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

ModelNode Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    // A. Extract Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Positions
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        // Normals
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        // Texture Coordinates
        if (mesh->mTextureCoords[0]) { 
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // B. Extract Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // C. Extract Material & Textures
    std::shared_ptr<Texture> diffuseMap = nullptr;

    float shininess = 32.0f;
    float specularIntensity = 0.0f;

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. Get the Diffuse Texture using the Global Asset Cache
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

            // Only get the filename and build custom path
            std::string assimpPath = str.C_Str();
            size_t lastSlash = assimpPath.find_last_of("/\\");
            std::string filename = (lastSlash != std::string::npos) ? assimpPath.substr(lastSlash + 1) : assimpPath;
            std::string texPath = directory + "/" + filename;

            if (assetManager) {
                diffuseMap = assetManager->getTexture(filename, texPath, "texture_diffuse");
            }
        }

        // 2. Get Shininess (The 'Ns' value in the .mtl file)
        material->Get(AI_MATKEY_SHININESS, shininess);

        // 3. Get Specular Intensity (The 'Ks' value in the .mtl file)
        aiColor3D specularColor(0.0f, 0.0f, 0.0f);
        if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS) {
            specularIntensity = (specularColor.r + specularColor.g + specularColor.b) / 3.0f;
        }
    }

    // D. Build the final ModelNode
    auto finalMesh = std::make_shared<Mesh>(vertices, indices);
    auto finalMat = std::make_shared<Material>(diffuseMap, 0, shininess, specularIntensity);

    return { finalMesh, finalMat };
}
