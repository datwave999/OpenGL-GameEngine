#pragma once

#include "Texture.h"
#include "Shader.h"
#include <memory>

class Material {
public:
    Material(const std::shared_ptr<Texture>& texture, int texUnit, float shininess = 0, float specularIntensity = 0);

    void Use(Shader* shader);

private:
    std::shared_ptr<Texture> diffuseMap;
    float shininess;
    float specularIntensity;

    int textureUnit;
};