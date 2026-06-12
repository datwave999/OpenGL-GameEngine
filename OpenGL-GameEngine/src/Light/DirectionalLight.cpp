#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float diffuseIntensity, float ambientIntensity) : 
    baseColor(color), diffuseIntensity(diffuseIntensity), ambientIntensity(ambientIntensity) {

    setDirection(direction);
    setColor(color);

    data.specular = glm::vec3(diffuseIntensity); // specular intensity matches diffuse intensity
}

void DirectionalLight::setDirection(glm::vec3 direction) {
    data.direction = glm::normalize(direction);
}

void DirectionalLight::setColor(glm::vec3 color) {
    baseColor = color;

    data.diffuse = baseColor * diffuseIntensity;
    data.ambient = baseColor * ambientIntensity;
}

void DirectionalLight::setIntensities(float diffuse, float ambient) {
    diffuseIntensity = diffuse;
    ambientIntensity = ambient;

    data.diffuse = baseColor * diffuseIntensity;
    data.ambient = baseColor * ambientIntensity;
}