#include "LightManager.h"

void LightManager::Initialize() {
    lightUBO = std::make_unique<Buffer>(GL_UNIFORM_BUFFER, sizeof(LightUBO), nullptr, GL_DYNAMIC_DRAW);
    lightUBO->bindBase(1);
}

void LightManager::setDirectionalLight(glm::vec3 direction, glm::vec3 color, float diffuseIntensity, float ambientIntensity)
{
    mainLight = std::make_unique<DirectionalLight>(direction, color, diffuseIntensity, ambientIntensity);
}

std::shared_ptr<PointLight> LightManager::getPointLight(std::string key, glm::vec3 position, glm::vec3 color, float radius, float diffuseIntensity, float ambientIntensity, float specularIntensity)
{
    auto it = pointLights.find(key);
    if (it != pointLights.end()) {
        if (std::shared_ptr<PointLight> light = it->second.lock()) {
            return light;
        }
    }

    std::shared_ptr<PointLight> newLight = std::make_shared<PointLight>(position, color, radius, diffuseIntensity, ambientIntensity, specularIntensity);
    pointLights[key] = newLight;

    return newLight;
}

void LightManager::Update() {
    if (!lightUBO) return;

    // 1. Pack the Directional Light
    uboData.directionalLight = mainLight->getLightData();

    // 2. Pack the Point Lights (With a safety limit of MAX_POINT_LIGHTS
    auto it = pointLights.begin();
    int active = 0;

    while (it != pointLights.end()) {
        if (auto light = it->second.lock()) {
            if (active < MAX_POINT_LIGHTS) {
                uboData.pointLights[active] = light->getLightData();
                active++;
            }
        }
        else {
            it = pointLights.erase(it);
        }
        ++it;
    }

    uboData.numPointLights = active;

    // 3. Send it to the GPU
    lightUBO->updateData(0, sizeof(LightUBO), &uboData);
}