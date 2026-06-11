#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float radius, float diffuseIntensity, float ambientIntensity, float specularIntensity) {
    baseColor = color;
    this->radius = radius;
    diffIntensity = diffuseIntensity;
    ambIntensity = ambientIntensity;
    specIntensity = specularIntensity;

    setPosition(position);
    setColor(color);   
    setRadius(radius); 
}

void PointLight::setPosition(glm::vec3 pos) {
    data.position = pos;
}

void PointLight::setColor(glm::vec3 color) {
    baseColor = color;

    data.diffuse = baseColor * diffIntensity;
    data.ambient = baseColor * ambIntensity;
    data.specular = baseColor * specIntensity;
}

void PointLight::setIntensities(float diffuse, float ambient, float specular) {
    diffIntensity = diffuse;
    ambIntensity = ambient;
    specIntensity = specular;

    data.diffuse = baseColor * diffIntensity;
    data.ambient = baseColor * ambIntensity;
    data.specular = baseColor * specIntensity;
}

void PointLight::setRadius(float newRadius) {
    if (newRadius <= 0.0f) newRadius = 0.0001f;

    radius = newRadius;
    
    data.constant = 1.0f;
    data.linear = 4.5f / radius;
    data.quadratic = 75.0f / (radius * radius);
}