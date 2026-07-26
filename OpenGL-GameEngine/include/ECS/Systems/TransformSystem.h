#pragma once

#include <entt/entt.hpp>
#include "ECS/Components/TransformComponent.h"

class TransformSystem {
public:
    void Update(entt::registry& registry);

    // Transform Mutators
    void Translate(TransformComponent& t, glm::vec3 offset);
    void MoveRelative(TransformComponent& t, glm::vec3 offset);
    void Rotate(TransformComponent& t, float degrees, glm::vec3 axis);
    void RotateRelative(TransformComponent& t, float degrees, glm::vec3 axis);

    // Setters
    void SetPosition(TransformComponent& t, glm::vec3 newPos);
    void SetRotation(TransformComponent& t, glm::quat newRotation);
    void SetRotation(TransformComponent& t, float pitchX, float yawY, float rollZ);
    void SetScale(TransformComponent& t, glm::vec3 newScale);

    // Directional Helpers
    glm::vec3 GetForward(const TransformComponent& t);
    glm::vec3 GetRight(const TransformComponent& t);
    glm::vec3 GetUp(const TransformComponent& t);
};