#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

#include "Resources/AssetData.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include "Graphics/VertexArray.h"

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    void Initialize();

    // Loaders (Return uint32_t handles to the flat vectors)
    uint32_t LoadShader(const std::string& key, const char* vert, const char* frag);

    uint32_t LoadTexture(const std::string& key, const std::string& path);
    uint32_t CreateTexture(const std::string& key, glm::vec4 color);

    uint32_t CreateMesh(const std::string& key, const std::vector<Vertex>& verts, const std::vector<GLuint>& indices);
    uint32_t CreateMesh(const std::string& key, const std::pair<std::vector<Vertex>, std::vector<GLuint>>& meshData) { return CreateMesh(key, meshData.first, meshData.second); }
    
    uint32_t CreateMaterial(const std::string& key, uint32_t diffTex, uint32_t specTex = (uint32_t)-1, float shininess = 1.0f, float specIntensity = 1.0f, uint32_t shaderHandle = 0);

    uint32_t CreateModel(const std::string& key, uint32_t meshHandle, uint32_t materialHandle);
    uint32_t LoadModel(const std::string& key, const std::string& path, uint32_t shaderHandle = 0, bool flipUVs = false);

    // Model Editors
    uint32_t SetModelNodeShader(uint32_t modelHandle, int nodeIndex, uint32_t newShaderHandle);

    // Fast Accessors for hot loops (No map lookups)
    Shader* GetShader(uint32_t handle);
    const MeshGPU* GetMesh(uint32_t handle) const;
    const TextureGPU* GetTexture(uint32_t handle) const;
    const MaterialAsset* GetMaterial(uint32_t handle) const;
    const ModelAsset* GetModel(uint32_t handle) const;

    // Defaults
    uint32_t GetDefaultTexture() const;
    uint32_t GetDefaultSpecularMap() const;

private:
    // Shaders
    std::vector<std::unique_ptr<Shader>> shaders;

    // Meshes
    std::vector<MeshGPU> meshes;
    std::vector<std::unique_ptr<Buffer>> buffers;
    std::vector<std::unique_ptr<VertexArray>> vertexArrays;

    // Materials
    std::vector<TextureGPU> textures;
    std::vector<MaterialAsset> materials;

    // Models
    std::vector<ModelAsset> models;

    // Caching map (String ID -> Handle)
    std::unordered_map<std::string, uint32_t> shaderMap;
    std::unordered_map<std::string, uint32_t> meshMap;
    std::unordered_map<std::string, uint32_t> textureMap;
    std::unordered_map<std::string, uint32_t> materialMap;
    std::unordered_map<std::string, uint32_t> modelMap;

    uint32_t defaultTextureHandle = 0;
    uint32_t defaultSpecularHandle = 0;

    // Assimp helpers
    void processNode(struct aiNode* node, const struct aiScene* scene, const std::string& directory, ModelAsset& outModel, uint32_t shaderHandle);
    ModelAsset::ModelNode processMesh(struct aiMesh* mesh, const struct aiScene* scene, const std::string& directory, uint32_t shaderHandle, uint32_t meshIndex);
};