#include "Material.h"

Material::Material(const std::shared_ptr<Texture>& texture, int texUnit, float shininess, float specularIntensity)
    : diffuseMap(texture), textureUnit(texUnit) {

    uboData.shininess = shininess;
    uboData.specularIntensity = specularIntensity;
    uboData.padding[0] = 0.0f;
    uboData.padding[1] = 0.0f;

    materialUBO = std::make_unique<Buffer>(GL_UNIFORM_BUFFER, sizeof(MaterialUBO), &uboData, GL_STATIC_DRAW);
}

void Material::Use(Shader* shader) {
    if (!diffuseMap) return;

    diffuseMap->Use(textureUnit);

    shader->setUniform(Uniform::texture1, textureUnit);

    materialUBO->bindBase(2);
}