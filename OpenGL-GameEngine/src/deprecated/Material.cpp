#include "deprecated/Material.h"

Material::Material(const std::shared_ptr<Texture>& diffMap, const std::shared_ptr<Texture>& specMap, int diffUnit, int specUnit, float shininess, float specularIntensity)
    : diffuseMap(diffMap), diffuseUnit(diffUnit), specularMap(specMap), specularUnit(specUnit) {

    uboData.shininess = shininess;
    uboData.specularIntensity = specularIntensity;
    uboData.padding[0] = 0.0f;
    uboData.padding[1] = 0.0f;

    materialUBO = std::make_unique<Buffer>(GL_UNIFORM_BUFFER, sizeof(MaterialUBO), &uboData, GL_STATIC_DRAW);
}

void Material::Use(Shader* shader) {

    if (diffuseMap) {
        diffuseMap->Use(diffuseUnit);
        shader->setUniform(Uniform::materialDiffuse, diffuseUnit);
    }

    if (specularMap) {
        specularMap->Use(specularUnit);
        shader->setUniform(Uniform::materialSpecular, specularUnit);
    }

    materialUBO->bindBase(2);
}