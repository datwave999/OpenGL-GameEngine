#include "Material.h"

Material::Material(const std::shared_ptr<Texture>& texture, int texUnit, float shininess, float specularIntensity)
    : diffuseMap(texture), shininess(shininess), specularIntensity(specularIntensity), textureUnit(texUnit) {
}

void Material::Use(Shader* shader) {
    if (!diffuseMap) return;

    diffuseMap->Use(textureUnit);

    shader->setUniform(Uniform::texture1, textureUnit);

    // shader->setUniform("material.shininess", shininess);
    // shader->setUniform("material.specularIntensity", specularIntensity);
}