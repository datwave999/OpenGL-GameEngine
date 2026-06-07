#pragma once
#pragma once
#include <unordered_map>
#include <string>

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

class AssetContainer {
public:
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, Material*> materials;
    std::unordered_map<std::string, Mesh*> meshes;
    std::unordered_map<std::string, Model*> models;

    void Clear();

    ~AssetContainer();
};