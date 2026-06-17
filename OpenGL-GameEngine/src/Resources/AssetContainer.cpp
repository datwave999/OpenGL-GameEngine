#include "AssetContainer.h"
#include <iostream>

// --- Smart Getters ---

std::shared_ptr<Texture> AssetContainer::getDefaultTexture() {
    std::string fallbackKey = "ENGINE_DEFAULT_CHECKERBOARD";

    auto it = textures.find(fallbackKey);
    if (it != textures.end()) {
        if (std::shared_ptr<Texture> sharedTex = it->second.lock()) {
            return sharedTex;
        }
    }

    unsigned int texID = Texture::CreateDefaultTexture();
    auto defaultTex = std::make_shared<Texture>(texID, "texture_diffuse");
    textures[fallbackKey] = defaultTex;

    return defaultTex;
}

std::shared_ptr<Texture> AssetContainer::getDefaultSpecularMap()
{
    return getTexture("ENGINE_DEFAULT_SPECULAR_MAP", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "texture_specular");
}


// Load from File
std::shared_ptr<Texture> AssetContainer::getTexture(const std::string& key, const std::string& filepath, const std::string& type) {
    auto it = textures.find(key);
    if (it != textures.end()) {
        if (std::shared_ptr<Texture> sharedTex = it->second.lock()) {
            return sharedTex; // Return cached version
        }
    }
    // Load from disk if it didn't exist or expired
    std::shared_ptr<Texture> newTex = std::make_shared<Texture>(filepath.c_str(), type.c_str());

    // If Texture Failed to Load
    if (newTex->getTextureID() == 0) {
        std::cout << "WARNING: Missing texture at '" << filepath << "'. Applying fallback." << std::endl;
        newTex = getDefaultTexture();
    }

    textures[key] = newTex;
    return newTex;
}

// Create Using Color
std::shared_ptr<Texture> AssetContainer::getTexture(const std::string& key, glm::vec4 color, const std::string& type) {
    auto it = textures.find(key);
    if (it != textures.end()) {
        if (std::shared_ptr<Texture> sharedTex = it->second.lock()) {
            return sharedTex; // Return cached version
        }
    }
    // Create from RGBA if doesn't exist
    std::shared_ptr<Texture> newTex = std::make_shared<Texture>(color, type.c_str());
    textures[key] = newTex;
    return newTex;
}

std::shared_ptr<Material> AssetContainer::getMaterial(const std::string& key, const std::shared_ptr<Texture>& diffuseMap, const std::shared_ptr<Texture>& specularMap, int diffUnit, int specUnit, float shininess, float specularIntensity) {
    auto it = materials.find(key);
    if (it != materials.end()) {
        if (std::shared_ptr<Material> sharedMat = it->second.lock()) {
            return sharedMat;
        }
    }

    // If material doesnt not have a specular map
    std::shared_ptr<Texture> finalSpecMap = specularMap;
    if (finalSpecMap == nullptr) {
        finalSpecMap = getDefaultSpecularMap();
    }

    std::shared_ptr<Material> newMat = std::make_shared<Material>(diffuseMap, finalSpecMap, diffUnit, specUnit, shininess, specularIntensity);
    materials[key] = newMat;
    return newMat;
}

// From Mesh and Material
std::shared_ptr<Model> AssetContainer::getModel(const std::string& key, const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) {
    auto it = models.find(key);
    if (it != models.end()) {
        if (std::shared_ptr<Model> sharedMod = it->second.lock()) {
            return sharedMod;
        }
    }
    std::shared_ptr<Model> newMod = std::make_shared<Model>(mesh, material);
    models[key] = newMod;
    return newMod;
}

// Import using Assimp
std::shared_ptr<Model> AssetContainer::getModel(const std::string& key, const std::string& filepath, bool flipUVs) {
    auto it = models.find(key);
    if (it != models.end()) {
        if (std::shared_ptr<Model> sharedMod = it->second.lock()) {
            return sharedMod;
        }
    }
    // Create the new model, passing 'this' so it can use the texture cache
    std::shared_ptr<Model> newMod = std::make_shared<Model>(filepath, this, flipUVs);

    models[key] = newMod;
    return newMod;
}

std::shared_ptr<Mesh> AssetContainer::getMesh(const std::string& key, const std::shared_ptr<Mesh>& newMesh) {
    meshes[key] = newMesh;
    return newMesh;
}

std::shared_ptr<Shader> AssetContainer::getShader(const std::string& key, const std::string& vertPath, const std::string& fragPath) {
    auto it = shaders.find(key);
    if (it != shaders.end()) {
        if (std::shared_ptr<Shader> shared = it->second.lock()) {
            return shared;
        }
    }

    std::shared_ptr<Shader> newShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    shaders[key] = newShader;
    return newShader;
}

// --- Utilities ---

void AssetContainer::CleanCache() {
    for (auto it = textures.begin(); it != textures.end(); ) {
        if (it->second.expired()) { it = textures.erase(it); }
        else { ++it; }
    }

    for (auto it = materials.begin(); it != materials.end(); ) {
        if (it->second.expired()) { it = materials.erase(it); }
        else { ++it; }
    }

    for (auto it = meshes.begin(); it != meshes.end(); ) {
        if (it->second.expired()) { it = meshes.erase(it); }
        else { ++it; }
    }

    for (auto it = models.begin(); it != models.end(); ) {
        if (it->second.expired()) { it = models.erase(it); }
        else { ++it; }
    }

    for (auto it = shaders.begin(); it != shaders.end(); ) {
        if (it->second.expired()) { it = shaders.erase(it); }
        else { ++it; }
    }
}

void AssetContainer::Clear() {
    meshes.clear();
    materials.clear();
    textures.clear();
    models.clear();
    shaders.clear();
}

AssetContainer::~AssetContainer() {
    Clear();
}