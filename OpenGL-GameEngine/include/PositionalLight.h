#pragma once
#include <glm/glm.hpp>

// An interface for PointLights and SpotLights
class PositionalLight {
public:
    virtual ~PositionalLight() = default;

    virtual void setPosition(glm::vec3 pos) = 0;
    virtual glm::vec3 getPosition() const = 0;

    virtual void setDirection(glm::vec3 direction) {}
    virtual glm::vec3 getDirection() const { return glm::vec3(0.0f); }
};