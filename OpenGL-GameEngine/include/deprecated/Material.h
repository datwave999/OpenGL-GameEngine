#pragma once

#include "deprecated/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include <memory>

struct MaterialUBO {
    float shininess;
    float specularIntensity;
    float padding[2];
};

class Material {
public:
    Material(const std::shared_ptr<Texture>& diffMap, const std::shared_ptr<Texture>& specMap, int diffUnit, int specUnit, float shininess = 32, float specularIntensity = 1.0f);

    void Use(Shader* shader);

private:
    std::shared_ptr<Texture> diffuseMap;
    std::shared_ptr<Texture> specularMap;

    // UBO to send shininess and specularIntensity
    std::unique_ptr<Buffer> materialUBO;
    MaterialUBO uboData;

    int diffuseUnit;
    int specularUnit;
};