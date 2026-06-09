#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    Transform();

    // Transform
    void Translate(glm::vec3 offset);
    void MoveRelative(glm::vec3 offset);
    void Rotate(float degrees, glm::vec3 axis);
    void RotateRelative(float degrees, glm::vec3 axis);

    // Setters
    void SetPosition(glm::vec3 newPos);
    void SetRotation(glm::quat newRotation);
    void SetRotation(float pitchX, float yawY, float rollZ);
    void SetScale(glm::vec3 newScale);

    // Getters
    const glm::vec3& GetPosition() const { return position; }
    const glm::vec3& GetScale() const { return scale; }
    const glm::quat& GetRotation() const { return rotation; }

    // Directional Helpers
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;

    // Matrices
    glm::mat4 getModelMatrix();
    glm::mat3 getNormalMatrix();

private:
    // Helper to update the matrices
    void UpdateMatrices();

    glm::mat4 ModelMatrix;
    glm::mat3 NormalMatrix;
    bool isTransformed;

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

