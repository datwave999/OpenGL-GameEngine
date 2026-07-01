#pragma once
#include "deprecated/LightData.h"
#include "deprecated/PositionalLight.h"

class SpotLight : public PositionalLight {
public:
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color,
        float innerCutOffAngle = 12.5f, float outerCutOffAngle = 17.5f,
        float radius = 50.0f, float diffuseIntensity = 1.0f,
        float ambientIntensity = 0.05f, float specularIntensity = 1.0f);

    void setPosition(glm::vec3 pos) override;
    glm::vec3 getPosition() const override { return data.position; }
    void setDirection(glm::vec3 direction) override;
    glm::vec3 getDirection() const override { return data.direction; }

    void setColor(glm::vec3 color);
    void setIntensities(float diffuse, float ambient, float specular);
    void setRadius(float newRadius);
    void setCutOffs(float innerAngle, float outerAngle);

    SpotLightData getLightData() const { return data; }

private:
    SpotLightData data;

    // Internal state
    glm::vec3 baseColor;
    float diffIntensity;
    float ambIntensity;
    float specIntensity;
    float radius;
};