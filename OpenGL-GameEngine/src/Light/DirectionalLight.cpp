#include "DirectionalLight.h"
#include <iostream>

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float diffuseIntensity, float ambientIntensity) : 
    baseColor(color), diffuseIntensity(diffuseIntensity), ambientIntensity(ambientIntensity) {

    setDirection(direction);
    setColor(color);
}

void DirectionalLight::setDirection(glm::vec3 direction) {
    if (length(direction) < 0.00001f) {
        std::cerr << "[ENGINE ERROR] DirectionalLight direction is zero!\n";
    } 
    data.direction = glm::normalize(direction);
}

void DirectionalLight::setColor(glm::vec3 color) {
    baseColor = color;

    data.diffuse = baseColor * diffuseIntensity;
    data.ambient = baseColor * ambientIntensity;
    data.specular = data.diffuse; // Match diffuse intensity for specular
}

void DirectionalLight::setIntensities(float diffuse, float ambient) {
    diffuseIntensity = diffuse;
    ambientIntensity = ambient;

    data.diffuse = baseColor * diffuseIntensity;
    data.ambient = baseColor * ambientIntensity;
}