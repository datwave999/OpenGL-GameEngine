#include "ECS/Systems/LightSystem.h"
#include <algorithm>
#include <vector>

LightSystem::LightSystem() {
    lightUBO = std::make_unique<Buffer>(GL_UNIFORM_BUFFER, sizeof(LightUBO), nullptr, GL_DYNAMIC_DRAW);
    lightUBO->bindBase(1);

    SetDirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), 0.0f, 0.0f);
}

void LightSystem::SetDirectionalLight(glm::vec3 direction, glm::vec3 color, float diff, float ambient) {
    directionalLight.direction = direction;
    directionalLight.ambient = color * ambient;
    directionalLight.diffuse = color * diff;
    directionalLight.specular = color;
}

void LightSystem::Update(entt::registry& registry, glm::vec3 cameraPos)
{
    // Set Direction Light
    uboData.directionalLight = directionalLight;

    // Clear the sorting arrays
    pointLightCache.clear();
    spotLightCache.clear();

    // Process Point Lights
    auto pointLightsView = registry.view<PointLightComponent, TransformComponent>();

    pointLightsView.each([&](auto entity, auto& pc, auto& tc) {
        glm::vec3 finalPos = tc.position;

        if (auto* offset = registry.try_get<LightOffsetComponent>(entity)) {
            finalPos += tc.rotation * offset->localOffset;
        }

        pc.position = finalPos;

        glm::vec3 diff = cameraPos - finalPos;
        float sqDist = glm::dot(diff, diff);
        pointLightCache.push_back({ entity, sqDist });
        });
    
    size_t pointLightCount = std::min(pointLightCache.size(), (size_t)MAX_POINT_LIGHTS);

    if (pointLightCount > 0) {
        // Partial Sort all the closest point lights
        std::partial_sort(pointLightCache.begin(), pointLightCache.begin() + pointLightCount, pointLightCache.end(), 
            [](const LightSortData& a, const LightSortData& b) { return a.sqDistance < b.sqDistance; }
        );

        // Update the data
        uboData.numPointLights = (int)pointLightCount;

        for (int i = 0; i < uboData.numPointLights; i++) {
            const auto& pc = registry.get<PointLightComponent>(pointLightCache[i].entity);

            uboData.pointLights[i].position = pc.position;
            uboData.pointLights[i].ambient = pc.color * pc.ambientIntensity;
            uboData.pointLights[i].diffuse = pc.color * pc.diffuseIntensity;
            uboData.pointLights[i].specular = pc.color * pc.specularIntensity;
            uboData.pointLights[i].constant = 1.0f;
            uboData.pointLights[i].linear = 4.5f / pc.radius;
            uboData.pointLights[i].quadratic = 75.0f / (pc.radius * pc.radius);
        }
    }
    else {
        uboData.numPointLights = 0;
    }

    // Process Spot Lights
    auto spotLightsView = registry.view<SpotLightComponent, TransformComponent>();

    spotLightsView.each([&](auto entity, auto& sc, auto& tc) {
        glm::vec3 finalPos = tc.position;
        glm::vec3 finalDir = tc.rotation * glm::vec3(0.0f, 0.0f, -1.0f);

        if (auto* offset = registry.try_get<LightOffsetComponent>(entity)) {
            finalPos += tc.rotation * offset->localOffset;
            finalDir = tc.rotation * glm::normalize(offset->directionOffset);
        }

        sc.position = finalPos;
        sc.direction = finalDir;

        glm::vec3 diff = cameraPos - finalPos;
        float sqDist = glm::dot(diff, diff);
        spotLightCache.push_back({ entity, sqDist });
        });

    size_t spotLightCount = std::min(spotLightCache.size(), (size_t)MAX_SPOT_LIGHTS);

    if (spotLightCount > 0) {
        // Partial Sort all the closest spot lights
        std::partial_sort(spotLightCache.begin(), spotLightCache.begin() + spotLightCount, spotLightCache.end(),
            [](const LightSortData& a, const LightSortData& b) { return a.sqDistance < b.sqDistance; }
        );

        // Update the data
        uboData.numSpotLights = (int)spotLightCount;

        for (int i = 0; i < uboData.numSpotLights; i++) {
            const auto& sc = registry.get<SpotLightComponent>(spotLightCache[i].entity);

            uboData.spotLights[i].position = sc.position;
            uboData.spotLights[i].cutOff = glm::cos(glm::radians(sc.innerCutOffAngle));;
            uboData.spotLights[i].direction = sc.direction;
            uboData.spotLights[i].outerCutOff = glm::cos(glm::radians(sc.outerCutOffAngle));
            uboData.spotLights[i].ambient = sc.color * sc.ambientIntensity;
            uboData.spotLights[i].constant = 1.0f;
            uboData.spotLights[i].diffuse = sc.color * sc.diffuseIntensity;
            uboData.spotLights[i].linear = 4.5f / sc.radius;
            uboData.spotLights[i].specular = sc.color * sc.specularIntensity;
            uboData.spotLights[i].quadratic = 75.0f / (sc.radius * sc.radius);
        }
    }
    else {
        uboData.numSpotLights = 0;
    }

    // Upload to GPU
    lightUBO->bind();
    lightUBO->updateData(0, sizeof(LightUBO), &uboData);
    lightUBO->unbind();
}


