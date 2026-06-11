#pragma once
#include "LightData.h"

class PointLight {
public:
    PointLight(glm::vec3 position, glm::vec3 color, float radius = 50.0f,
                float diffuseIntensity = 1.0f,
                float ambientIntensity = 0.05f,
                float specularIntensity = 1.0f);

    void setPosition(glm::vec3 pos);
    void setColor(glm::vec3 color);
    void setIntensities(float diffuse, float ambient, float specular);

    void setRadius(float newRadius);

    glm::vec3 getPosition() const { return data.position; }

    PointLightData getLightData() const { return data; }

private:
    PointLightData data;

    // Internal state
    glm::vec3 baseColor;
    float diffIntensity;
    float ambIntensity;
    float specIntensity;
    float radius;
};