#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

class AssetContainer {
public:
    ~AssetContainer();

    std::shared_ptr<Texture> getTexture(const std::string& key, const std::string& filepath, const std::string& type);
    std::shared_ptr<Material> getMaterial(const std::string& key, const std::shared_ptr<Texture>& texture, int texUnit);
    std::shared_ptr<Model> getModel(const std::string& key, const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
    std::shared_ptr<Model> getModel(const std::string& key, const std::string& filepath, bool flipUVs = true);
    std::shared_ptr<Mesh> getMesh(const std::string& key, const std::shared_ptr<Mesh>& newMesh);

    // --- Utilities ---
    void CleanCache();
    void Clear();

private:
    std::unordered_map<std::string, std::weak_ptr<Texture>> textures;
    std::unordered_map<std::string, std::weak_ptr<Material>> materials;
    std::unordered_map<std::string, std::weak_ptr<Mesh>> meshes;
    std::unordered_map<std::string, std::weak_ptr<Model>> models;
};