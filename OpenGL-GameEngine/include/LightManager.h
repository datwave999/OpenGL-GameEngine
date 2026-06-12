#pragma once
#include "Buffer.h"
#include "LightData.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

class LightManager {
public:
    void Initialize();

    void setDirectionalLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
                        glm::vec3 color = glm::vec3(1.0f),
                        float diffuseIntensity = 1.0f,
                        float ambientIntensity = 0.1f);

    std::shared_ptr<PointLight> getPointLight(glm::vec3 position, glm::vec3 color, float radius = 50.0f,
                                            float diffuseIntensity = 1.0f,
                                            float ambientIntensity = 0.05f,
                                            float specularIntensity = 1.0f);

    std::shared_ptr<SpotLight> getSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color,
                                        float innerCutOffAngle = 12.5f, float outerCutOffAngle = 17.5f,
                                        float radius = 50.0f, float diffuseIntensity = 1.0f,
                                        float ambientIntensity = 0.05f, float specularIntensity = 1.0f);

    void UpdateData(glm::vec3 cameraPos);

private:
    // Lights
    std::unique_ptr<DirectionalLight> mainLight;
    std::vector<std::weak_ptr<PointLight>> pointLights;
    std::vector<std::weak_ptr<SpotLight>> spotLights;

    // Light UBO data
    std::unique_ptr<Buffer> lightUBO;
    LightUBO uboData;

    // Light distance-culling storage
    std::vector<std::shared_ptr<PointLight>> alivePointLights;
    std::vector<std::shared_ptr<SpotLight>> aliveSpotLights;
};