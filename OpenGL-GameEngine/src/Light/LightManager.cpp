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

void LightManager::UpdateData(glm::vec3 cameraPos) {
    if (!lightUBO) return;

    // Pack the Directional Light
    if (mainLight) {
        uboData.directionalLight = mainLight->getLightData();
    }
    else {
        uboData.directionalLight = DirectionalLightData{};
    }

    // --- POINT LIGHTS ---
    
    // A. Gather living lights and pop the dead ones
    alivePointLights.clear();
    
    for (int i = 0; i < pointLights.size();) {
        if (auto light = pointLights[i].lock()) {
            alivePointLights.push_back(light);
            i++;
        }
        else {
            // Swap and Pop
            pointLights[i] = pointLights.back();
            pointLights.pop_back();
        }
    }

    // B. partially sort to get the lowest distance ones that are in our limit of MAX_POINT_LIGHTS
    int pointLightCount = std::min((int)alivePointLights.size(), MAX_POINT_LIGHTS);

    if (pointLightCount > 0) {
        std::partial_sort(alivePointLights.begin(),
            alivePointLights.begin() + pointLightCount,
            alivePointLights.end(),
            [&cameraPos](const std::shared_ptr<PointLight>& a, const std::shared_ptr<PointLight>& b) {
                glm::vec3 diffA = a->getPosition() - cameraPos;
                glm::vec3 diffB = b->getPosition() - cameraPos;
                return glm::dot(diffA, diffA) < glm::dot(diffB, diffB);
            });
    }

    // C. pack the data for only the non-culled lights
    for (int i = 0; i < pointLightCount; ++i) {
        uboData.pointLights[i] = alivePointLights[i]->getLightData();
    }
    uboData.numPointLights = pointLightCount;



    // --- SPOT LIGHTS ---

    // A. Gather living lights and pop the dead ones
    aliveSpotLights.clear();

    for (int i = 0; i < spotLights.size();) {
        if (auto light = spotLights[i].lock()) {
            aliveSpotLights.push_back(light);
            i++;
        }
        else {
            // Swap and Pop
            spotLights[i] = spotLights.back();
            spotLights.pop_back();
        }
    }

    // B. partially sort to get the lowest distance ones that are in our limit of MAX_SPOT_LIGHTS
    int spotLightCount = std::min((int)aliveSpotLights.size(), MAX_SPOT_LIGHTS);

    if (spotLightCount > 0) {
        std::partial_sort(aliveSpotLights.begin(),
            aliveSpotLights.begin() + spotLightCount,
            aliveSpotLights.end(),
            [&cameraPos](const std::shared_ptr<SpotLight>& a, const std::shared_ptr<SpotLight>& b) {
                glm::vec3 diffA = a->getPosition() - cameraPos;
                glm::vec3 diffB = b->getPosition() - cameraPos;
                return glm::dot(diffA, diffA) < glm::dot(diffB, diffB);
            });
    }

    // C. pack the data for only the non-culled lights
    for (int i = 0; i < spotLightCount; ++i) {
        uboData.spotLights[i] = aliveSpotLights[i]->getLightData();
    }
    uboData.numSpotLights = spotLightCount;



    // --- UPLOAD DATA TO GPU ---
    lightUBO->updateData(0, sizeof(LightUBO), &uboData);
}