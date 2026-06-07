#include"AssetContainer.h"

void AssetContainer::Clear() {
    meshes.clear();
    materials.clear();
    textures.clear();
    models.clear();
}

// The destructor automatically calls Clear() when the engine shuts down
AssetContainer::~AssetContainer() {
    Clear();
}