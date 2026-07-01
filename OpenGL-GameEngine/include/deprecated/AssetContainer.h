#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <initializer_list>

#include "deprecated/Texture.h"
#include "deprecated/Material.h"
#include "deprecated/Mesh.h"
#include "deprecated/Model.h"

class AssetContainer {
public:
    ~AssetContainer();

    std::shared_ptr<Texture> getDefaultTexture();
    std::shared_ptr<Texture> getDefaultSpecularMap();
    std::shared_ptr<Texture> getTexture(const std::string& key, const std::string& filepath, const std::string& type = "texture_diffuse");
    std::shared_ptr<Texture> getTexture(const std::string& key, glm::vec4 color, const std::string& type = "texture_diffuse");
    std::shared_ptr<Material> getMaterial(const std::string& key, const std::shared_ptr<Texture>& diffMap, const std::shared_ptr<Texture>& specMap, int diffUnit, int specUnit, float shininess = 32, float specularIntensity = 1.0f);
    std::shared_ptr<Model> getModel(const std::string& key, const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
    std::shared_ptr<Model> getModel(const std::string& key, const std::string& filepath, bool flipUVs = false);
    std::shared_ptr<Mesh> getMesh(const std::string& key, const std::shared_ptr<Mesh>& newMesh);
    std::shared_ptr<Shader> getShader(const std::string& key, const std::string& vertPath, const std::string& fragPath);

    // --- Utilities ---
    void CleanCache();
    void Clear();

private:
    std::unordered_map<std::string, std::weak_ptr<Texture>> textures;
    std::unordered_map<std::string, std::weak_ptr<Material>> materials;
    std::unordered_map<std::string, std::weak_ptr<Mesh>> meshes;
    std::unordered_map<std::string, std::weak_ptr<Model>> models;
    std::unordered_map<std::string, std::weak_ptr<Shader>> shaders;
};