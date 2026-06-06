#pragma once

#include "Texture.h"
#include "Shader.h"

class Material {
public:
    Material(Texture* texture, int texUnit, float shininess = 0, float specularIntensity = 0);

    void Use(Shader* shader);

private:
    Texture* diffuseMap;
    float shininess;
    float specularIntensity;

    int textureUnit;
};