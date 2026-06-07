#include"AssetContainer.h"

void AssetContainer::Clear() {
    // Order matters! Delete high-level objects before base resources
    for (auto& pair : meshes) { delete pair.second; }
    for (auto& pair : materials) { delete pair.second; }
    for (auto& pair : textures) { delete pair.second; }
    for (auto& pair : models) { delete pair.second; }

    meshes.clear();
    materials.clear();
    textures.clear();
    models.clear();
}

// The destructor automatically calls Clear() when the engine shuts down
AssetContainer::~AssetContainer() {
    Clear();
}