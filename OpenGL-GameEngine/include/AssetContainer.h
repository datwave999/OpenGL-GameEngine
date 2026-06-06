#pragma once
#pragma once
#include <unordered_map>
#include <string>

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class AssetContainer {
public:
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, Material*> materials;
    std::unordered_map<std::string, Mesh*> meshes;

    void Clear();

    ~AssetContainer();
};