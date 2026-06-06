#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    Transform();

    void Translate(glm::vec3 offset);
    void MoveRelative(glm::vec3 offset);

    void Scale(glm::vec3 newScale);

    void Rotate(float degrees, glm::vec3 axis);

    void SetRotation(glm::quat newRotation);
    void SetRotation(float pitchX, float yawY, float rollZ);

    glm::quat GetRotation() const;
    glm::mat4 getModelMatrix() const;

private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

