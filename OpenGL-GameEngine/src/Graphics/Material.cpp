#include "Material.h"

Material::Material(Texture* texture, int texUnit, float shininess, float specularIntensity)
    : diffuseMap(texture), shininess(shininess), specularIntensity(specularIntensity), textureUnit(texUnit) {
}

void Material::Use(Shader* shader) {
    if (!diffuseMap) return;

    diffuseMap->Use(textureUnit);

    shader->setUniform("theTexture", textureUnit);

    // shader->setUniform("material.shininess", shininess);
    // shader->setUniform("material.specularIntensity", specularIntensity);
}