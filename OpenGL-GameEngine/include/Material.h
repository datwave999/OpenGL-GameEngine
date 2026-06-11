#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Buffer.h"
#include <memory>

struct MaterialUBO {
    float shininess;
    float specularIntensity;
    float padding[2];
};

class Material {
public:
    Material(const std::shared_ptr<Texture>& texture, int texUnit, float shininess = 32, float specularIntensity = 1.0f);

    void Use(Shader* shader);

private:
    std::shared_ptr<Texture> diffuseMap;

    // UBO to send shininess and specularIntensity
    std::unique_ptr<Buffer> materialUBO;
    MaterialUBO uboData;

    int textureUnit;
};