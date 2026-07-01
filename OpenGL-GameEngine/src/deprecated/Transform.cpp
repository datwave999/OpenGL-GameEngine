#include "deprecated/Transform.h"


// Constructor

Transform::Transform() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    ModelMatrix = glm::mat4(1.0f);
    isTransformed = true;
}

// Transform

void Transform::Translate(glm::vec3 offset) {
    position += offset;

    isTransformed = true;
}

void Transform::MoveRelative(glm::vec3 offset) {
    position += rotation * offset;

    isTransformed = true;
}

void Transform::Rotate(float degrees, glm::vec3 axis) {
    glm::quat rot = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
    rotation = rot * rotation;
    rotation = glm::normalize(rotation);

    isTransformed = true;
}

void Transform::RotateRelative(float degrees, glm::vec3 axis)
{
    glm::quat rot = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
    rotation = rotation * rot;
    rotation = glm::normalize(rotation);

    isTransformed = true;
}

// Setters

void Transform::SetPosition(glm::vec3 newPos)
{
    position = newPos;

    isTransformed = true;
}

void Transform::SetRotation(glm::quat newRotation) {
    rotation = newRotation;

    isTransformed = true;
}

void Transform::SetRotation(float pitchX, float yawY, float rollZ) {
    glm::vec3 euler(glm::radians(pitchX), glm::radians(yawY), glm::radians(rollZ));
    rotation = glm::quat(euler);

    isTransformed = true;
}

void Transform::SetScale(glm::vec3 newScale) {
    scale = newScale;

    isTransformed = true;
}

// Directional Helpers

glm::vec3 Transform::GetForward() const
{
    return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 Transform::GetRight() const
{
    return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Transform::GetUp() const
{
    return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}


// Final Model & Normal Matrices

void Transform::UpdateMatrices() {
    if (isTransformed) {
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, position);
        ModelMatrix = ModelMatrix * glm::mat4_cast(rotation);
        ModelMatrix = glm::scale(ModelMatrix, scale);

        NormalMatrix = glm::transpose(glm::inverse(glm::mat3(ModelMatrix)));

        isTransformed = false;
    }
}

glm::mat4 Transform::getModelMatrix() {
    UpdateMatrices();
    return ModelMatrix;
}

glm::mat3 Transform::getNormalMatrix() {
    UpdateMatrices();
    return NormalMatrix;
}