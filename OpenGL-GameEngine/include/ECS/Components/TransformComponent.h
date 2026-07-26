#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct TransformComponent {
    glm::vec3 position{ 0.0f };
    glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
    glm::vec3 scale{ 1.0f };
    bool dirty = true;
    glm::mat4 modelMatrix{ 1.0f };
    glm::mat3 normalMatrix{ 1.0f };
};