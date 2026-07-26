#pragma once
#include <glad/glad.h>
#include <vector>
#include <cstdint>
#include <memory>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct MeshGPU {
    GLuint VAO;
    int indexCount = 0;
};

struct TextureGPU {
    GLuint textureID = 0;
};

struct MaterialAsset {
    uint32_t shaderHandle;
    uint32_t diffuseTextureHandle;
    uint32_t specularTextureHandle;
    float shininess;
    float specularIntensity;
    int diffuseUnit;
    int specularUnit;
    GLuint materialUBO;  // GPU UBO for this material
};

struct ModelAsset {
    struct ModelNode {
        uint32_t meshHandle;
        uint32_t materialHandle;
    };

    std::vector<ModelNode> nodes;
};
