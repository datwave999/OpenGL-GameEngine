#pragma once
#include <entt/entt.hpp>
#include "Graphics/Buffer.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/LightComponents.h"
#include <memory>

class LightSystem {
public:
    LightSystem();
    void SetDirectionalLight(glm::vec3 direction, glm::vec3 color, float diff, float ambient);
    void Update(entt::registry& registry, glm::vec3 cameraPos);

private:
    std::unique_ptr<Buffer> lightUBO;
    LightUBO uboData;
    DirectionalLightComponent directionalLight;

    // Caching vectors to avoid per-frame heap allocations
    struct LightSortData {
        entt::entity entity;
        float sqDistance;
    };
    std::vector<LightSortData> pointLightCache;
    std::vector<LightSortData> spotLightCache;
};