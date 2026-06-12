#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color,
    float innerCutOffAngle, float outerCutOffAngle, float radius,
    float diffuseIntensity, float ambientIntensity, float specularIntensity)
{
    baseColor = color;
    this->radius = radius;
    diffIntensity = diffuseIntensity;
    ambIntensity = ambientIntensity;
    specIntensity = specularIntensity;

    setPosition(position);
    setDirection(direction);
    setColor(color);
    setRadius(radius);
    setCutOffs(innerCutOffAngle, outerCutOffAngle);
}

void SpotLight::setPosition(glm::vec3 pos) { data.position = pos; }

void SpotLight::setDirection(glm::vec3 dir) { data.direction = glm::normalize(dir); }

void SpotLight::setColor(glm::vec3 color) {
    baseColor = color;
    data.diffuse = baseColor * diffIntensity;
    data.ambient = baseColor * ambIntensity;
    data.specular = baseColor * specIntensity;
}

void SpotLight::setIntensities(float diffuse, float ambient, float specular) {
    diffIntensity = diffuse;
    ambIntensity = ambient;
    specIntensity = specular;
    setColor(baseColor);
}

void SpotLight::setRadius(float newRadius) {
    if (newRadius <= 0.0f) newRadius = 0.0001f;
    radius = newRadius;
    data.constant = 1.0f;
    data.linear = 4.5f / radius;
    data.quadratic = 75.0f / (radius * radius);
}

void SpotLight::setCutOffs(float innerAngle, float outerAngle) {
    // Pre-calculate cosine values here so the shader doesn't have to
    data.cutOff = glm::cos(glm::radians(innerAngle));
    data.outerCutOff = glm::cos(glm::radians(outerAngle));
}