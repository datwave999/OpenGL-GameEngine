#include "ECS/Systems/TransformSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void TransformSystem::Update(entt::registry& registry) {
    auto view = registry.view<TransformComponent>();

    view.each([&](auto entity, auto& tc) {
        if (tc.dirty) {
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), tc.position);
            glm::mat4 rotate = glm::toMat4(tc.rotation);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), tc.scale);
            tc.modelMatrix = translate * rotate * scale;
            tc.normalMatrix = glm::transpose(glm::inverse(glm::mat3(tc.modelMatrix)));
            tc.dirty = false;
        }
    });
}

void TransformSystem::Translate(TransformComponent& t, glm::vec3 offset) {
    t.position += offset;
    t.dirty = true;
}

void TransformSystem::MoveRelative(TransformComponent& t, glm::vec3 offset) {
    t.position += t.rotation * offset;
    t.dirty = true;
}

void TransformSystem::Rotate(TransformComponent& t, float degrees, glm::vec3 axis) {
    glm::quat rot = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
    t.rotation = rot * t.rotation;
    t.rotation = glm::normalize(t.rotation);
    t.dirty = true;
}

void TransformSystem::RotateRelative(TransformComponent& t, float degrees, glm::vec3 axis) {
    glm::quat rot = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
    t.rotation = t.rotation * rot;
    t.rotation = glm::normalize(t.rotation);
    t.dirty = true;
}

void TransformSystem::SetPosition(TransformComponent& t, glm::vec3 newPos) {
    t.position = newPos;
    t.dirty = true;
}

void TransformSystem::SetRotation(TransformComponent& t, glm::quat newRotation) {
    t.rotation = newRotation;
    t.dirty = true;
}

void TransformSystem::SetRotation(TransformComponent& t, float pitchX, float yawY, float rollZ) {
    glm::vec3 euler(glm::radians(pitchX), glm::radians(yawY), glm::radians(rollZ));
    t.rotation = glm::quat(euler);
    t.dirty = true;
}

void TransformSystem::SetScale(TransformComponent& t, glm::vec3 newScale) {
    t.scale = newScale;
    t.dirty = true;
}

glm::vec3 TransformSystem::GetForward(const TransformComponent& t) {
    return t.rotation * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 TransformSystem::GetRight(const TransformComponent& t) {
    return t.rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 TransformSystem::GetUp(const TransformComponent& t) {
    return t.rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}