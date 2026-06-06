#include "Transform.h"

Transform::Transform() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

void Transform::Translate(glm::vec3 offset) {
    position += offset;
}

void Transform::MoveRelative(glm::vec3 offset) {
    position += rotation * offset;
}

void Transform::Scale(glm::vec3 newScale) {
    scale = newScale;
}

void Transform::Rotate(float degrees, glm::vec3 axis) {
    glm::quat rot = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
    rotation = rot * rotation;
    rotation = glm::normalize(rotation);
}

void Transform::SetRotation(glm::quat newRotation) {
    rotation = newRotation;
}

void Transform::SetRotation(float pitchX, float yawY, float rollZ) {
    glm::vec3 euler(glm::radians(pitchX), glm::radians(yawY), glm::radians(rollZ));
    rotation = glm::quat(euler);
}

glm::quat Transform::GetRotation() const {
    return rotation;
}

glm::mat4 Transform::getModelMatrix() const {
    glm::mat4 mat = glm::mat4(1.0f);

    mat = glm::translate(mat, position);
    mat = mat * glm::mat4_cast(rotation);
    mat = glm::scale(mat, scale);

    return mat;
}