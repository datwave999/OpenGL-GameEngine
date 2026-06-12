#include "LightManager.h"

void LightManager::Initialize() {
    lightUBO = std::make_unique<Buffer>(GL_UNIFORM_BUFFER, sizeof(LightUBO), nullptr, GL_DYNAMIC_DRAW);
    lightUBO->bindBase(1);
}

void LightManager::setDirectionalLight(glm::vec3 direction, glm::vec3 color, float diffuseIntensity, float ambientIntensity)
{
    mainLight = std::make_unique<DirectionalLight>(direction, color, diffuseIntensity, ambientIntensity);
}

std::shared_ptr<PointLight> LightManager::getPointLight(glm::vec3 position, glm::vec3 color, float radius, float diffuseIntensity, float ambientIntensity, float specularIntensity)
{
    std::shared_ptr<PointLight> newLight = std::make_shared<PointLight>(position, color, radius, diffuseIntensity, ambientIntensity, specularIntensity);
    pointLights.push_back(newLight);
    return newLight;
}

std::shared_ptr<SpotLight> LightManager::getSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float innerCutOffAngle, float outerCutOffAngle, float radius, float diffuseIntensity, float ambientIntensity, float specularIntensity)
{
    std::shared_ptr<SpotLight> newLight = std::make_shared<SpotLight>(position, direction, color, innerCutOffAngle, outerCutOffAngle, radius, diffuseIntensity, ambientIntensity, specularIntensity);
    spotLights.push_back(newLight);
    return newLight;
}

void LightManager::UpdateData() {
    if (!lightUBO) return;

    // 1. Pack the Directional Light
    if (mainLight) {
        uboData.directionalLight = mainLight->getLightData();
    }
    else {
        uboData.directionalLight = DirectionalLightData{};
    }

    // 2. Pack the Point Lights (With a safety limit of MAX_POINT_LIGHTS)
    int active = 0;

    for (int i = 0; i < pointLights.size(); ) {
        if (auto light = pointLights[i].lock()) {
            if (active < MAX_POINT_LIGHTS) {
                uboData.pointLights[active] = light->getLightData();
                active++;
            }
            i++;
        }
        else {
            // Swap and Pop for O(1) deletion
            pointLights[i] = pointLights.back();
            pointLights.pop_back();
        }
    }

    uboData.numPointLights = active;

    // 2. Pack the Spot Lights (With a safety limit of MAX_SPOT_LIGHTS)
    active = 0;

    for (int i = 0; i < spotLights.size(); ) {
        if (auto light = spotLights[i].lock()) {
            if (active < MAX_SPOT_LIGHTS) {
                uboData.spotLights[active] = light->getLightData();
                active++;
            }
            i++;
        }
        else {
            // Swap and Pop for O(1) deletion
            spotLights[i] = spotLights.back();
            spotLights.pop_back();
        }
    }

    uboData.numSpotLights = active;

    // 3. Send it to the GPU
    lightUBO->updateData(0, sizeof(LightUBO), &uboData);
}