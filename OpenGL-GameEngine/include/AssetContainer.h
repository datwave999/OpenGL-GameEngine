#pragma once
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
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
    std::unordered_map<std::string, std::shared_ptr<Model>> models;

    void Clear();

    ~AssetContainer();
};