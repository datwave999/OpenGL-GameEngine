#include "Resources/AssetManager.h"

#include <iostream>
#include <stdexcept>
#include <STB/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {
    for (auto& tex : textures) {
        if (tex.textureID != 0) glDeleteTextures(1, &tex.textureID);
    }
}

void AssetManager::Initialize() {
    defaultTextureHandle = CreateTexture("default_diffuse", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    defaultSpecularHandle = CreateTexture("default_specular", glm::vec4(1.0f));
}

uint32_t AssetManager::LoadShader(const std::string& key, const char* vert, const char* frag) {
    if (shaderMap.find(key) != shaderMap.end()) return shaderMap[key];
    uint32_t handle = (uint32_t)shaders.size();
    shaders.push_back(std::make_unique<Shader>(vert, frag));
    shaderMap[key] = handle;
    return handle;
}

uint32_t AssetManager::LoadTexture(const std::string& key, const std::string& path) {
    if (textureMap.find(key) != textureMap.end()) return textureMap[key];

    TextureGPU texGPU;
    glGenTextures(1, &texGPU.textureID);
    glBindTexture(GL_TEXTURE_2D, texGPU.textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 2) format = GL_RG;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        glDeleteTextures(1, &texGPU.textureID);
        texGPU.textureID = 0;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    if (data) stbi_image_free(data);

    uint32_t handle = (uint32_t)textures.size();
    textures.push_back(std::move(texGPU));
    textureMap[key] = handle;
    return handle;
}

uint32_t AssetManager::CreateTexture(const std::string& key, glm::vec4 color) {
    if (textureMap.find(key) != textureMap.end()) return textureMap[key];

    TextureGPU texGPU;
    glGenTextures(1, &texGPU.textureID);
    glBindTexture(GL_TEXTURE_2D, texGPU.textureID);

    float pixel[] = { color.r, color.g, color.b, color.a };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, pixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    uint32_t handle = (uint32_t)textures.size();
    textures.push_back(std::move(texGPU));
    textureMap[key] = handle;
    return handle;
}

uint32_t AssetManager::CreateMesh(const std::string& key, const std::vector<Vertex>& verts, const std::vector<GLuint>& indices) {
    if (meshMap.find(key) != meshMap.end()) return meshMap[key];

    MeshGPU mesh;

    std::unique_ptr<VertexArray> VAO = std::make_unique<VertexArray>();
    std::unique_ptr<Buffer> VBO = std::make_unique<Buffer>(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
    std::unique_ptr<Buffer> EBO = std::make_unique<Buffer>(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    mesh.VAO = VAO->getID();
    mesh.indexCount = static_cast<int>(indices.size());

    VAO->addVertexBuffer(*VBO, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Position));
    VAO->addVertexBuffer(*VBO, 1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Normal));
    VAO->addVertexBuffer(*VBO, 2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, TexCoords));
    VAO->setIndexBuffer(*EBO);

    vertexArrays.push_back(std::move(VAO));
    buffers.push_back(std::move(VBO));
    buffers.push_back(std::move(EBO));

    uint32_t handle = (uint32_t)meshes.size();
    meshes.push_back(std::move(mesh));
    meshMap[key] = handle;
    return handle;
}

uint32_t AssetManager::CreateMaterial(const std::string& key, uint32_t diffTex, uint32_t specTex, float shininess, float specIntensity, uint32_t shaderHandle) {
    if (materialMap.find(key) != materialMap.end()) return materialMap[key];

    MaterialAsset mat;
    mat.shaderHandle = shaderHandle;
    mat.diffuseTextureHandle = diffTex;
    mat.specularTextureHandle = (specTex == (uint32_t)-1) ? defaultSpecularHandle : specTex;
    mat.shininess = shininess;
    mat.specularIntensity = specIntensity;

    mat.diffuseUnit = 1;
    mat.specularUnit = 2;

    float uboData[4] = { shininess, specIntensity, 0.0f, 0.0f };

    std::unique_ptr<Buffer> materialUBO = std::make_unique<Buffer>(GL_UNIFORM_BUFFER, sizeof(uboData), uboData, GL_STATIC_DRAW);

    mat.materialUBO = materialUBO->getBufferID();

    buffers.push_back(std::move(materialUBO));

    uint32_t handle = (uint32_t)materials.size();
    materials.push_back(std::move(mat));
    materialMap[key] = handle;
    return handle;
}

uint32_t AssetManager::CreateModel(const std::string& key, uint32_t meshHandle, uint32_t materialHandle) {
    if (modelMap.find(key) != modelMap.end()) return modelMap[key];

    ModelAsset model;
    model.nodes.push_back({ meshHandle, materialHandle });

    uint32_t handle = (uint32_t)models.size();
    models.push_back(std::move(model));
    modelMap[key] = handle;
    return handle;
}

uint32_t AssetManager::LoadModel(const std::string& key, const std::string& path, uint32_t shaderHandle, bool flipUVs) {
    if (modelMap.find(key) != modelMap.end()) return modelMap[key];

    Assimp::Importer importer;
    unsigned int importFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals;
    if (flipUVs) importFlags |= aiProcess_FlipUVs;

    const aiScene* scene = importer.ReadFile(path, importFlags);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return (uint32_t)-1;
    }

    std::string directory = path.substr(0, path.find_last_of("/\\"));

    ModelAsset model;
    processNode(scene->mRootNode, scene, directory, model, shaderHandle);

    uint32_t handle = (uint32_t)models.size();
    models.push_back(std::move(model));
    modelMap[key] = handle;
    return handle;
}

void AssetManager::processNode(aiNode* node, const aiScene* scene, const std::string& directory, ModelAsset& outModel, uint32_t shaderHandle) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        outModel.nodes.push_back(processMesh(mesh, scene, directory, shaderHandle, node->mMeshes[i]));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, directory, outModel, shaderHandle);
    }
}

ModelAsset::ModelNode AssetManager::processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory, uint32_t shaderHandle, uint32_t meshIndex) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (mesh->HasNormals()) vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }

    uint32_t diffMap = GetDefaultTexture();
    uint32_t specMap = GetDefaultSpecularMap();
    float shininess = 32.0f;
    float specIntensity = 1.0f;

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString matName;

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            std::string assimpPath = str.C_Str();
            std::string filename = assimpPath.substr(assimpPath.find_last_of("/\\") + 1);
            diffMap = LoadTexture(directory + "/" + filename, directory + "/" + filename);
        }
        if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            aiString str;
            material->GetTexture(aiTextureType_SPECULAR, 0, &str);
            std::string assimpPath = str.C_Str();
            std::string filename = assimpPath.substr(assimpPath.find_last_of("/\\") + 1);
            specMap = LoadTexture(directory + "/" + filename, directory + "/" + filename);
        }
        material->Get(AI_MATKEY_SHININESS, shininess);
        aiColor3D specularColor(0.0f, 0.0f, 0.0f);
        if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS) {
            specIntensity = (specularColor.r + specularColor.g + specularColor.b) / 3.0f;
        }
    }

    std::string meshKey = directory + "_MESH_" + std::to_string(meshIndex);
    std::string matKey = directory + "_MATERIAL_" + std::to_string(mesh->mMaterialIndex) + "_SHADER_" + std::to_string(shaderHandle);

    uint32_t meshHandle = CreateMesh(meshKey, vertices, indices);
    uint32_t matHandle = CreateMaterial(matKey, diffMap, specMap, shininess, specIntensity, shaderHandle);
    return { meshHandle, matHandle };
}


// Model Editors
uint32_t AssetManager::SetModelNodeShader(uint32_t modelHandle, int nodeIndex, uint32_t newShaderHandle)
{
    if (modelHandle >= models.size() || nodeIndex < 0 || nodeIndex >= models[modelHandle].nodes.size()) {
        return (uint32_t)-1; // Returns invalid handle if not found
    }

    uint32_t matHandle = models[modelHandle].nodes[nodeIndex].materialHandle;
    materials[matHandle].shaderHandle = newShaderHandle;
    return modelHandle;
}

// Getters
Shader* AssetManager::GetShader(uint32_t handle) { return handle < shaders.size() ? shaders[handle].get() : nullptr; }
const MeshGPU* AssetManager::GetMesh(uint32_t handle) const { return handle < meshes.size() ? &meshes[handle] : nullptr; }
const TextureGPU* AssetManager::GetTexture(uint32_t handle) const { return handle < textures.size() ? &textures[handle] : nullptr; }
const MaterialAsset* AssetManager::GetMaterial(uint32_t handle) const { return handle < materials.size() ? &materials[handle] : nullptr; }
const ModelAsset* AssetManager::GetModel(uint32_t handle) const { return handle < models.size() ? &models[handle] : nullptr; }

uint32_t AssetManager::GetDefaultTexture() const { return defaultTextureHandle; }
uint32_t AssetManager::GetDefaultSpecularMap() const { return defaultSpecularHandle; }
