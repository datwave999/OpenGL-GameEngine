#pragma once
#include "deprecated/LightData.h"

class DirectionalLight {
public:
    DirectionalLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
                    glm::vec3 color = glm::vec3(1.0f),
                    float diffuseIntensity = 1.0f,
                    float ambientIntensity = 0.1f);

    void setDirection(glm::vec3 dir);
    void setColor(glm::vec3 color);
    void setIntensities(float diffuse, float ambient);

    // Returns the raw, padded struct ready for the UBO
    DirectionalLightData getLightData() const { return data; }

private:
    DirectionalLightData data;

    glm::vec3 baseColor;
    float diffuseIntensity;
    float ambientIntensity;
};